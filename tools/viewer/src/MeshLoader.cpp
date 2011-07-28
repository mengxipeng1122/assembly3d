/*
 * Copyright (c) 2011 Peter Vasil
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "MeshLoader.h"
#include <libxml/xmlreader.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void processNode(xmlTextReaderPtr reader, MeshLoader* loader) {

    const char *name = (const char*)xmlTextReaderConstName(reader);
    assert (name != NULL);

    int nodeType = xmlTextReaderNodeType(reader);
    if(nodeType != 1)
    {
        return;
    }

    if(strcmp("Vertices", name) == 0)
    {
        GLsizei count = atoi ( (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "count") );
        GLsizei attributes = atoi ( (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "attributes") );
        loader->vertices( count, attributes );
    }
    else if(strcmp("Attribute", name) == 0)
    {
        const GLchar* name = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "name");
        GLsizei size = atoi ( (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "size") );
        //const char* typeName = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "type");
        //assert(strcmp("FLOAT", typeName) == 0);
        GLenum type = GL_FLOAT;
        GLsizei typeSize = 4;
        loader->attribute( name, size, type, typeSize );
    }
    else if(strcmp("Triangles", name) == 0)
    {
        const char* typeName = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "type");
        GLsizei groups = atoi ( (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "groups") );

        GLenum type = GL_UNSIGNED_SHORT;
        GLsizei typeSize = 2;
        if(strcmp("UNSIGNED_BYTE", typeName) == 0)
        {
            type = GL_UNSIGNED_BYTE;
            typeSize = 1;
        }
        else if(strcmp("UNSIGNED_INT", typeName) == 0)
        {
            type = GL_UNSIGNED_INT;
            typeSize = 4;
        }
        loader->triangles(type, typeSize, groups);
    }
    else if(strcmp("Group", name) == 0)
    {
        const GLchar* name = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "name");
        GLsizei count = atoi ( (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "count") );

        loader->group( name, count );
    }
}

MeshLoader::MeshLoader(Mesh *m,
                       const char *metafilename,
                       const char *datafilename,
                       ProgramSimple* p)
    : mesh(m), prog(p)
{
    file = fopen(datafilename, "r");
    assert(file != NULL);

    xmlTextReaderPtr reader = xmlReaderForFile(metafilename, NULL, 0);
    assert(reader != NULL);
    int ret = xmlTextReaderRead(reader);
    while (ret == 1)
    {
        processNode(reader, this);
        ret = xmlTextReaderRead(reader);
    }
    xmlFreeTextReader(reader);
    assert(ret == 0);

    finish();

}

void MeshLoader::vertices(GLsizei count, GLsizei attributes)
{
    //printf("<Vertices count=\"%d\" attributes=\"%d\">\n", count, attributes);

    mesh->nVertices = count;
    mesh->nAttributes = attributes;

    index = 0;

    mesh->buffers = new GLuint[attributes+1];
    mesh->attrSizes = new GLsizei[attributes];
    mesh->attrTypes = new GLenum[attributes];
    mesh->attrTypeSizes = new GLsizei[attributes];
    mesh->attrNames.clear();
    
#ifdef A3D_GL_USE_VAO
    glGenVertexArrays(1, &mesh->vertexArray);
    glBindVertexArray(mesh->vertexArray);
#endif
}

void MeshLoader::attribute(const GLchar *name, GLsizei size, GLenum type, GLsizei typeSize)
{
//        printf("<Attribute name=\"%s\" size=\"%d\" type=\"%d\">\n", name, size, type);

    GLsizei stride = size*typeSize;

    GLuint idx = index++;
    mesh->attrSizes[idx] = size;
    mesh->attrTypeSizes[idx] = typeSize;
    mesh->attrTypes[idx] = type;

    mesh->attrNames.push_back(name);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->nVertices*stride, (GLvoid*) 0, GL_STATIC_DRAW);

#ifdef A3D_GL_USE_VAO
    GLint attrLoc = -1;
    if(strcmp("POSITION", name) == 0)
    {
        attrLoc = prog->position();
    }
    else if(strcmp("NORMAL", name) == 0)
    {
        attrLoc = prog->normal();
    }
    else if(strcmp("TEXCOORD", name) == 0)
    {
        attrLoc = prog->texCoord();
    }
    glEnableVertexAttribArray(attrLoc);
    glVertexAttribPointer(attrLoc, size, type, GL_FALSE, 0, (GLvoid*) 0);
#endif
    
    char* data = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    fread(data, stride, mesh->nVertices, file);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mesh->buffers[idx] = buffer;
}

void MeshLoader::triangles(GLenum type, GLsizei typeSize, GLuint groups)
{
    //printf("<Triangles type=\"%d\" groups=\"%d\">\n", type, groups);
    index = 0;

    mesh->nGroups = groups;
    mesh->nTriangles = new GLsizei[groups+1];
    mesh->indexSize = typeSize;
    mesh->indexType = type;
    mesh->nTotalTriangles = 0;

    mesh->nTriangles[index++] = 0;
    mesh->groupNames.clear();
}

void MeshLoader::group(const GLchar *name, GLsizei count)
{
//        printf("<Group name=\"%s\" count=\"%d\">\n", name, count);

    GLuint idx = index++;

    mesh->groupNames.push_back(name);

    mesh->nTotalTriangles += count;
    mesh->nTriangles[idx] = mesh->nTotalTriangles;

}

void MeshLoader::finish()
{
    //printf("finish\n");
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->nTotalTriangles*3*mesh->indexSize, (GLvoid*) 0, GL_STATIC_DRAW);

    char* data = (char*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    fread(data, mesh->indexSize, mesh->nTotalTriangles*3, file);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

#ifdef A3D_GL_USE_VAO
    glBindVertexArray(0);
#endif
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    mesh->buffers[mesh->nAttributes] = buffer;
    fclose(file);
}
