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

#include "MeshIO.h"
#include "WizUtils.h"
#include <fstream>

using namespace WizUtils;

MeshIO::MeshIO()
{
}

MeshIO::~MeshIO()
{
}

bool MeshIO::load(Mesh* mesh, const char* file, const char* binaryFile)
{
    int numVertices = 0;
//    int numAttributes = 0;
    int numGroups = 0;
    int numIndices = 0;
    int groupIndex = 0;
    
    int numTriangles = 0;

    mesh->destroy();
    
//    m_MeshPath = file;
    mesh->setMeshPath(file);
    Mesh::MeshFormat& format = mesh->getMeshFormat();
    
    XmlParser xml;
    if(!xml.loadFile(file))
        return false;
    
    
    xml.pushTag("Mesh");
    {
        numVertices = xml.getAttribute("Vertices", "count", 0);
        
        numGroups = xml.getAttribute("Triangles", "groups", 0);
        format.indexType = xml.getAttribute("Triangles", "type", "UNSIGNED_INT");
        
        xml.pushTag("Vertices");
        {
            format.attributeCount = xml.getNumTags("Attribute");
            
            for(int i = 0; i < format.attributeCount; ++i)
            {
                format.attributeName.push_back(xml.getAttribute("Attribute", "name", "", i));
                format.attributeSize.push_back(xml.getAttribute("Attribute", "size", 0, i));
                format.attributeType.push_back(xml.getAttribute("Attribute", "type", "FLOAT", i));
            }
            
        }
        xml.popTag();
        
        xml.pushTag("Triangles");
        {
            for(int i = 0; i < numGroups; ++i)
            {
                Group g;
                g.name = xml.getAttribute("Group", "name", "", i);
                g.triangleCount = xml.getAttribute("Group", "count", 0, i);
                
                if(groupIndex == 0)
                {
                    g.startIndex = numIndices;
                }
                else
                {
                    g.startIndex = numIndices+1;
                }
                numIndices += g.triangleCount * 3;
                ++groupIndex;
                
                numTriangles += g.triangleCount;
                
                mesh->addGroup(g);
                
            }
            mesh->setNumTriangles(numTriangles);
        }
        xml.popTag();
        
//        loadBinaryFile = !(xml.tagExists("Data"));
        
        // -------------------------------------------------
        // Load binary file
        // -------------------------------------------------
        format.isBinary = true;

        std::ifstream fin(binaryFile, std::ios::binary);

        fin.seekg(0);

        std::vector<float> verts;
        std::vector<float> texCoords;
        std::vector<float> normals;
        std::vector<float> tangents;
        std::vector<float> bitangents;

        bool loadTexture = false;
        bool loadNormal = false;
        bool loadTangent = false;
        bool loadBitangent = false;

        int index = 0;
        int startLoop = 0;
        int endLoop = 0;
        int idx = -1;
        int aSize = format.attributeCount;
        idx = mesh->getAttributeIndexWithName("POSITION");

        if(idx > -1 && idx < aSize)
        {
            startLoop = numVertices*idx;
            endLoop = numVertices *(idx+1);

            for(int i = startLoop; i < endLoop; ++i)
            {
                float px = 0.0f;
                float py = 0.0f;
                float pz = 0.0f;

                fin.read((char *)(&px), sizeof(px));
                fin.read((char *)(&py), sizeof(py));
                fin.read((char *)(&pz), sizeof(pz));

                verts.push_back(px);
                verts.push_back(py);
                verts.push_back(pz);
            }

        }
        idx = mesh->getAttributeIndexWithName("NORMAL");
        if(idx > -1 && idx < aSize)
        {
            loadNormal = true;
            index = 0;
            startLoop = numVertices*idx;
            endLoop = numVertices*(idx+1);
            for(int i = startLoop; i < endLoop; ++i)
            {
                float nx = 0.0f;
                float ny = 0.0f;
                float nz = 0.0f;

                fin.read((char *)(&nx), sizeof(nx));
                fin.read((char *)(&ny), sizeof(ny));
                fin.read((char *)(&nz), sizeof(nz));

                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);
            }
        }
        idx = mesh->getAttributeIndexWithName("TEXTURE");
        if(idx > -1 && idx < aSize)
        {
            loadTexture = true;
            index = 0;
            startLoop = numVertices*idx;
            endLoop = numVertices*(idx+1);
            for(int i = startLoop; i < endLoop; ++i)
            {
                float tx = 0.0f;
                float ty = 0.0f;

                fin.read((char *)(&tx), sizeof(tx));
                fin.read((char *)(&ty), sizeof(ty));

                texCoords.push_back(tx);
                texCoords.push_back(ty);
            }
        }
        idx = mesh->getAttributeIndexWithName("TANGENT");
        if(idx > -1 && idx < aSize)
        {
            loadTangent = true;
            index = 0;
            startLoop = numVertices*idx;
            endLoop = numVertices*(idx+1);
            for(int i = startLoop; i < endLoop; ++i)
            {
                float tanx = 0.0f;
                float tany = 0.0f;
                float tanz = 0.0f;

                fin.read((char *)(&tanx), sizeof(tanx));
                fin.read((char *)(&tany), sizeof(tany));
                fin.read((char *)(&tanz), sizeof(tanz));

                tangents.push_back(tanx);
                tangents.push_back(tany);
                tangents.push_back(tanz);
            }
        }
        idx = mesh->getAttributeIndexWithName("BITANGENT");
        if(idx > -1 && idx < aSize)
        {
            loadBitangent = true;
            index = 0;
            startLoop = numVertices*idx;
            endLoop = numVertices*(idx+1);
            for(int i = startLoop; i < endLoop; ++i)
            {
                float btanx = 0.0f;
                float btany = 0.0f;
                float btanz = 0.0f;

                fin.read((char *)(&btanx), sizeof(btanx));
                fin.read((char *)(&btany), sizeof(btany));
                fin.read((char *)(&btanz), sizeof(btanz));

                bitangents.push_back(btanx);
                bitangents.push_back(btany);
                bitangents.push_back(btanz);
            }
        }
        if(format.indexType.compare("UNSIGNED_INT") == 0)
        {
            for(int i = 0; i < numIndices; ++i)
            {
                unsigned int index = 0;
                fin.read((char *)(&index), sizeof(index));
                mesh->addIndex(index);
            }
        }
        else if(format.indexType.compare("UNSIGNED_SHORT") == 0)
        {
            for(int i = 0; i < numIndices; ++i)
            {
                unsigned short index = 0;
                fin.read((char *)(&index), sizeof(index));
                mesh->addIndex((unsigned int)index);
            }
        }
        else if(format.indexType.compare("UNSIGNED_BYTE") == 0)
        {
            for(int i = 0; i < numIndices; ++i)
            {
                unsigned char index = 0;
                fin.read((char *)(&index), sizeof(index));
                mesh->addIndex((unsigned int)index);
            }
        }

        for(int i = 0; i < numVertices; ++i)
        {
            Vertex vert = {
                {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f}
            };
            vert.position[0] = verts[i * 3 + 0];
            vert.position[1] = verts[i * 3 + 1];
            vert.position[2] = verts[i * 3 + 2];

            if(loadNormal)
            {

                vert.normal[0] = normals[i * 3 + 0];
                vert.normal[1] = normals[i * 3 + 1];
                vert.normal[2] = normals[i * 3 + 2];
            }
            if(loadTexture)
            {

                vert.texCoord[0] = texCoords[i * 2 + 0];
                vert.texCoord[1] = texCoords[i * 2 + 1];
            }
            if(loadTangent)
            {

                vert.tangent[0] = tangents[i * 3 + 0];
                vert.tangent[1] = tangents[i * 3 + 1];
                vert.tangent[2] = tangents[i * 3 + 2];
            }
            if(loadBitangent)
            {

                vert.bitangent[0] = bitangents[i * 3 + 0];
                vert.bitangent[1] = bitangents[i * 3 + 1];
                vert.bitangent[2] = bitangents[i * 3 + 2];
            }
            mesh->addVertex(vert);
        }
            
    }
    xml.popTag();
    
    mesh->calculateBounds();
    mesh->hasPositions(mesh->getAttributeIndexWithName("POSITION") != -1 ? true : false);
    mesh->hasNormals(mesh->getAttributeIndexWithName("NORMAL") != -1 ? true : false);
    mesh->hasTexCoords(mesh->getAttributeIndexWithName("TEXTURE") != -1 ? true : false);
    mesh->hasTangents(mesh->getAttributeIndexWithName("TANGENT") != -1 ? true : false);
    mesh->hasBitangents(mesh->getAttributeIndexWithName("BITANGENT") != -1 ? true : false);
    
    return true;
    
}

void MeshIO::dumpTxt(Mesh* mesh, const char* outFilePath)
{
    // -------------------------------------------------------------------------------------------
    // Root: Mesh
    // -------------------------------------------------------------------------------------------
    std::stringstream ss;
    ss << "----------------------------------------------\n";
    ss << "Mesh: " << WizUtils::FileUtils::getFileName(mesh->getMeshPath()) << "\n";
    ss << "----------------------------------------------\n";

    // -------------------------------------------------------------------------------------------
    // Vertices
    // -------------------------------------------------------------------------------------------
    ss << "Vertices: " <<  "count=" << (int)mesh->getNumberOfVertices() << " ";
    ss << "attributes=" << mesh->getMeshFormat().attributeCount << "\n";

    // -------------------------------------------------------------------------------------------
    // Attributes
    // -------------------------------------------------------------------------------------------
    std::vector<int> attribIndices;
    getAttributeIndices(mesh, attribIndices);

    for(int attrIndex = 0; attrIndex < mesh->getMeshFormat().attributeCount; ++attrIndex)
    {
        // -------------------------------------------------------------------------------------------
        // Element
        // -------------------------------------------------------------------------------------------
        ss << "Attribute, name=" << mesh->getMeshFormat().attributeName[attribIndices[attrIndex]].c_str() << " ";
        ss << "size=" << mesh->getMeshFormat().attributeSize[attribIndices[attrIndex]] << "\n";

    }
    ss << std::endl;
    // -------------------------------------------------------------------------------------------
    // Triangles
    // -------------------------------------------------------------------------------------------
    ss << "Triangles: " << "groups=" << (int)mesh->getNumberOfGroups() << "\n";

    // -------------------------------------------------------------------------------------------
    // Groups
    // -------------------------------------------------------------------------------------------
    for(unsigned int groupIndex = 0; groupIndex < mesh->getNumberOfGroups(); ++groupIndex)
    {
        // -------------------------------------------------------------------------------------------
        // Group
        // -------------------------------------------------------------------------------------------
        const MeshWiz::Group& g = mesh->getGroup(groupIndex);

        ss << "Group: name=" << g.name.c_str() << " count=" << g.triangleCount << "\n";
    }
    ss << std::endl;
    // -------------------------------------------------------------------------------------------
    // Data
    // -------------------------------------------------------------------------------------------
    ss << "Data:" << std::endl;

    std::stringstream data;
    int idx = -1;
    int aSize = mesh->getMeshFormat().attributeCount;
    idx = mesh->getAttributeIndexWithName("POSITION");
    if(idx > -1 && idx < aSize)
    {
        data << "Positions:" << "\n";

        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);

            for(int vertSizePosIndex = 0; vertSizePosIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizePosIndex)
            {
                data << vert.position[vertSizePosIndex] << " ";
            }
            data << std::endl;
        }
    }
    idx = mesh->getAttributeIndexWithName("NORMAL");
    if(idx > -1 && idx < aSize)
    {
        data << "Normals:" << std::endl;

        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);

            for(int vertSizeNormalIndex = 0; vertSizeNormalIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeNormalIndex)
            {
                data << vert.normal[vertSizeNormalIndex] << " ";
            }
            data << std::endl;
        }
    }
    idx = mesh->getAttributeIndexWithName("TEXTURE");
    if(idx > -1 && idx < aSize)
    {
        data << "TexCoords:" << std::endl;

        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);

            for(int vertSizeTexIndex = 0; vertSizeTexIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeTexIndex)
            {
                data << vert.texCoord[vertSizeTexIndex] << " ";
            }
            data << std::endl;
        }
    }
    idx = mesh->getAttributeIndexWithName("TANGENT");
    if(idx > -1 && idx < aSize)
    {
        data << "Tangents:" << std::endl;

        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);

            for(int vertSizeTexIndex = 0; vertSizeTexIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeTexIndex)
            {
                data << vert.tangent[vertSizeTexIndex] << " ";
            }
            data << std::endl;
        }
    }
    idx = mesh->getAttributeIndexWithName("BITANGENT");
    if(idx > -1 && idx < aSize)
    {
        data << "Bitangents:" << std::endl;

        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);

            for(int vertSizeTexIndex = 0; vertSizeTexIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeTexIndex)
            {
                data << vert.bitangent[vertSizeTexIndex] << " ";
            }
            data << std::endl;
        }
    }

    // Triangles
    data << "Triangles:" << std::endl;

    for(int triangleIndex = 0; triangleIndex < mesh->getNumberOfTriangles(); ++triangleIndex)
    {
        const unsigned int* pTriangle = 0;
        pTriangle = mesh->getTriangle(triangleIndex);
        data << (int)pTriangle[0] << " " << (int)pTriangle[1] << " " << (int)pTriangle[2] << std::endl;
    }

    ss << data.str();


//    xml.saveFile(outFilePath);
    std::ofstream fout(outFilePath);
    fout << ss.str();
    fout.close();

}

void MeshIO::saveFile(Mesh* mesh, const char* outFilePath, const char* binaryFilePath)
{
    XmlParser xml;
    xml.addXmlDeclaration();
    // -------------------------------------------------------------------------------------------
    // Root: Mesh
    // -------------------------------------------------------------------------------------------
    xml.addTag("Mesh");
    xml.addAttribute("Mesh", "xmlns", "http://xml.qu.tu-berlin.de/assembly/mesh", 0);
    xml.addAttribute("Mesh", "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance", 0);
    xml.addAttribute("Mesh", "xsi:schemaLocation", "http://xml.qu.tu-berlin.de/assembly/mesh mesh.xsd", 0);
    xml.pushTag("Mesh");
    {
        // -------------------------------------------------------------------------------------------
        // Vertices
        // -------------------------------------------------------------------------------------------
  
        xml.addTag("Vertices");
        xml.addAttribute("Vertices", "count", (int)mesh->getNumberOfVertices(), 0);
        xml.addAttribute("Vertices", "attributes", mesh->getMeshFormat().attributeCount, 0);
        
        xml.pushTag("Vertices");
        {
            // -------------------------------------------------------------------------------------------
            // Attributes
            // -------------------------------------------------------------------------------------------
            std::vector<int> attribIndices;
            getAttributeIndices(mesh, attribIndices);

            for(int attrIndex = 0; attrIndex < mesh->getMeshFormat().attributeCount; ++attrIndex)
            {
                // -------------------------------------------------------------------------------------------
                // Element
                // -------------------------------------------------------------------------------------------
                xml.addTag("Attribute", false);
                xml.addAttribute("Attribute", "name", mesh->getMeshFormat().attributeName[attribIndices[attrIndex]].c_str(), attrIndex);
                xml.addAttribute("Attribute", "size", mesh->getMeshFormat().attributeSize[attribIndices[attrIndex]], attrIndex);
                xml.addAttribute("Attribute", "type", mesh->getMeshFormat().attributeType[attribIndices[attrIndex]].c_str(), attrIndex);
            }
        }
        xml.popTag();
        
        // -------------------------------------------------------------------------------------------
        // Triangles
        // -------------------------------------------------------------------------------------------
        xml.addTag("Triangles");
        xml.addAttribute("Triangles","groups", (int)mesh->getNumberOfGroups(), 0);
        xml.addAttribute("Triangles", "type", mesh->getMeshFormat().indexType.c_str(), 0);
        
        xml.pushTag("Triangles");
        {
            // -------------------------------------------------------------------------------------------
            // Groups
            // -------------------------------------------------------------------------------------------
            for(unsigned int groupIndex = 0; groupIndex < mesh->getNumberOfGroups(); ++groupIndex)
            {
                // -------------------------------------------------------------------------------------------
                // Group
                // -------------------------------------------------------------------------------------------
                xml.addTag("Group", false);

                const MeshWiz::Group& g = mesh->getGroup(groupIndex);

                xml.addAttribute("Group", "name", g.name.c_str(), groupIndex);
                xml.addAttribute("Group", "count", g.triangleCount, groupIndex);
            }
        }
    }
    xml.popTag();
    
    xml.saveFile(outFilePath);
    
    // -------------------------------------------------------------------------------------------
    // Data
    // -------------------------------------------------------------------------------------------
    std::ofstream fout(binaryFilePath, std::ios::binary);
    
    int idx = -1;
    int aSize = mesh->getMeshFormat().attributeCount;
    idx = mesh->getAttributeIndexWithName("POSITION");
    if(idx > -1 && idx < aSize)
    {
        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);
            
            for(int vertSizePosIndex = 0; vertSizePosIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizePosIndex)
            {
                fout.write((char *)(&vert.position[vertSizePosIndex]), sizeof(vert.position[vertSizePosIndex]));
            }   
        }
    }
    idx = mesh->getAttributeIndexWithName("NORMAL");
    if(idx > -1 && idx < aSize)
    {
        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);
            
            for(int vertSizeNormalIndex = 0; vertSizeNormalIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeNormalIndex)
            {
                fout.write((char *)(&vert.normal[vertSizeNormalIndex]), sizeof(vert.normal[vertSizeNormalIndex]));
            }
        }
    }
    idx = mesh->getAttributeIndexWithName("TEXTURE");
    if(idx > -1 && idx < aSize)
    {
   
        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);
            
            for(int vertSizeTexIndex = 0; vertSizeTexIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeTexIndex)
            {
                fout.write((char *)(&vert.texCoord[vertSizeTexIndex]), sizeof(vert.texCoord[vertSizeTexIndex]));
            }
        }
    }
    idx = mesh->getAttributeIndexWithName("TANGENT");
    if(idx > -1 && idx < aSize)
    {
   
        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);
            
            for(int vertSizeTexIndex = 0; vertSizeTexIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeTexIndex)
            {
                fout.write((char *)(&vert.tangent[vertSizeTexIndex]), sizeof(vert.tangent[vertSizeTexIndex]));
            }
        }
    }
    idx = mesh->getAttributeIndexWithName("BITANGENT");
    if(idx > -1 && idx < aSize)
    {
   
        for(unsigned int vertexIndex = 0; vertexIndex < mesh->getNumberOfVertices(); ++vertexIndex)
        {
            const Vertex& vert = mesh->getVertex(vertexIndex);
            
            for(int vertSizeTexIndex = 0; vertSizeTexIndex < mesh->getMeshFormat().attributeSize[idx]; ++vertSizeTexIndex)
            {
                fout.write((char *)(&vert.bitangent[vertSizeTexIndex]), sizeof(vert.bitangent[vertSizeTexIndex]));
            }
        }
    }
	fout.flush();
    
    if(mesh->getMeshFormat().indexType.compare("UNSIGNED_INT") == 0)
    {
        for(int triangleIndex = 0; triangleIndex < mesh->getNumberOfTriangles(); ++triangleIndex)
        {
            const unsigned int* pTriangle = mesh->getTriangle(triangleIndex);
            unsigned int index0 = pTriangle[0];
            unsigned int index1 = pTriangle[1];
            unsigned int index2 = pTriangle[2];
            fout.write((char* )(&index0), sizeof(index0));
            fout.write((char* )(&index1), sizeof(index1));
            fout.write((char* )(&index2), sizeof(index2));
        }
      
    }
    else if (mesh->getMeshFormat().indexType.compare("UNSIGNED_SHORT")==0)
    {
        for(int triangleIndex = 0; triangleIndex < mesh->getNumberOfTriangles(); ++triangleIndex)
        {
            const unsigned int* pTriangle = mesh->getTriangle(triangleIndex);
            unsigned short index0 = (unsigned short)pTriangle[0];
            unsigned short index1 = (unsigned short)pTriangle[1];
            unsigned short index2 = (unsigned short)pTriangle[2];
            fout.write((char* )(&index0), sizeof(index0));
            fout.write((char* )(&index1), sizeof(index1));
            fout.write((char* )(&index2), sizeof(index2));
        }
       
    }
    else if (mesh->getMeshFormat().indexType.compare("UNSIGNED_BYTE")==0)
    {
        for(int triangleIndex = 0; triangleIndex < mesh->getNumberOfTriangles(); ++triangleIndex)
        {
            const unsigned int* pTriangle = mesh->getTriangle(triangleIndex);
            unsigned char index0 = (unsigned char)pTriangle[0];
            unsigned char index1 = (unsigned char)pTriangle[1];
            unsigned char index2 = (unsigned char)pTriangle[2];
            fout.write((char* )(&index0), sizeof(index0));
            fout.write((char* )(&index1), sizeof(index1));
            fout.write((char* )(&index2), sizeof(index2));
        }
    }
    
    fout.flush();
    fout.close();

}

void MeshIO::getAttributeIndices(Mesh* mesh, std::vector<int>& indices)
{
    if(mesh->getAttributeIndexWithName("POSITION")!= -1)
        indices.push_back(mesh->getAttributeIndexWithName("POSITION"));
    if(mesh->getAttributeIndexWithName("NORMAL")!= -1)
        indices.push_back(mesh->getAttributeIndexWithName("NORMAL"));
    if(mesh->getAttributeIndexWithName("TEXTURE")!= -1)
        indices.push_back(mesh->getAttributeIndexWithName("TEXTURE"));
    if(mesh->getAttributeIndexWithName("TANGENT")!= -1)
        indices.push_back(mesh->getAttributeIndexWithName("TANGENT"));
    if(mesh->getAttributeIndexWithName("BITANGENT")!= -1)
        indices.push_back(mesh->getAttributeIndexWithName("BITANGENT"));

}
