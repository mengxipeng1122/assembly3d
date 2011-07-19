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

#include "Mesh.h"
#include <libxml/xmlreader.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Loader
{
    Loader(Mesh *m, const char *datafilename) : mesh(m)
    {
        file = fopen(datafilename, "r");
        assert(file != NULL);
    }

    void vertices(GLsizei count, GLsizei attributes)
    {
        printf("<Vertices count=\"%d\" attributes=\"%d\">\n", count, attributes);

        mesh->nVertices = count;
        mesh->nAttributes = attributes;

        index = 0;

        mesh->buffers = new GLuint[attributes+1];

        glGenVertexArrays(1, &mesh->vertexArray);
        glBindVertexArray(mesh->vertexArray);
    }

    void attribute(const GLchar *name, GLsizei size, GLenum type, GLsizei typeSize)
    {
        printf("<Attribute name=\"%s\" size=\"%d\" type=\"%d\">\n", name, size, type);

        GLsizei stride = size*typeSize;

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, mesh->nVertices*stride, (GLvoid*) 0, GL_STATIC_DRAW);
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);

        char* data = (char*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        fread(data, stride, mesh->nVertices, file);
        glUnmapBuffer(GL_ARRAY_BUFFER);

        mesh->buffers[index++] = buffer;
    }

    void triangles(GLenum type, GLsizei typeSize, GLuint groups)
    {
        printf("<Triangles type=\"%d\" groups=\"%d\">\n", type, groups);
        index = 0;

        mesh->nGroups = groups;
        mesh->nTriangles = new GLsizei[groups+1];
        mesh->indexSize = typeSize;
        mesh->indexType = type;
        mesh->nTotalTriangles = 0;

        mesh->nTriangles[index++] = 0;
    }

    void group(const GLchar *name, GLsizei count)
    {
        printf("<Group name=\"%s\" count=\"%d\">\n", name, count);

        mesh->nTotalTriangles += count;
        mesh->nTriangles[index++] = mesh->nTotalTriangles;
    }

    void finish()
    {
        printf("finish\n");
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->nTotalTriangles*3*mesh->indexSize, (GLvoid*) 0, GL_STATIC_DRAW);

        char* data = (char*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        fread(data, mesh->indexSize, mesh->nTotalTriangles*3, file);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

        glBindVertexArray(0);

        mesh->buffers[mesh->nAttributes-1] = buffer;
        fclose(file);
    }

private:
    GLuint index;
    FILE * file;
    Mesh *mesh;
};

static void processNode(xmlTextReaderPtr reader, Loader& loader) {

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
        loader.vertices( count, attributes );
    }
    else if(strcmp("Attribute", name) == 0)
    {
        const GLchar* name = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "name");
        GLsizei size = atoi ( (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "size") );
        //const char* typeName = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "type");
        //assert(strcmp("FLOAT", typeName) == 0);
        GLenum type = GL_FLOAT;
        GLsizei typeSize = 4;
        loader.attribute( name, size, type, typeSize );
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
        loader.triangles(type, typeSize, groups);
    }
    else if(strcmp("Group", name) == 0)
    {
        const GLchar* name = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "name");
        GLsizei count = atoi ( (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "count") );

        loader.group( name, count );
    }
}

Mesh::Mesh(const char* metafilename, const char* datafilename) {

    Loader loader(this, datafilename);

    xmlTextReaderPtr reader = xmlReaderForFile(metafilename, NULL, 0);
    assert(reader != NULL);
    int ret = xmlTextReaderRead(reader);
    while (ret == 1)
    {
        processNode(reader, loader);
        ret = xmlTextReaderRead(reader);
    }
    xmlFreeTextReader(reader);
    assert(ret == 0);

    loader.finish();
}

Mesh::~Mesh() {

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(nAttributes, buffers);
    delete buffers;
    delete nTriangles;
}

void Mesh::draw() {
    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, 3*nTotalTriangles, indexType, (GLvoid*) 0);
    glBindVertexArray(0);
}

void Mesh::draw(GLuint index) {
    glBindVertexArray(vertexArray);
    GLsizei count = nTriangles[index+1] - nTriangles[index];
    glDrawElements(GL_TRIANGLES, 3*nTriangles[index+1], indexType, (GLvoid*) (nTriangles[index]*3*indexSize));
    glBindVertexArray(0);
}
