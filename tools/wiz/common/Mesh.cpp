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

#include "A3DIncludes.h"
#include "Mesh.h"
#include <limits>
#include <cmath>
#include <algorithm>

using namespace assembly3d;

Mesh::Mesh()
:
m_numVertices(0),
m_numTriangles(0),
m_hasPositions(false),
m_hasNormals(false),
m_hasTexCoords(false),
m_hasTangents(false),
m_hasBitangents(false),
m_width(0.0f),
m_height(0.0f),
m_length(0.0f),
m_radius(0.0f)
{
    m_center[0] = 0.0f;
    m_center[1] = 0.0f;
    m_center[2] = 0.0f;
    
    m_extent[0] = 0.0f;
    m_extent[1] = 0.0f;
    m_extent[2] = 0.0f;
}

Mesh::Mesh(const Mesh &m)
{
    m_positions = m.m_positions;
    m_normals = m.m_normals;
    m_texCoords = m.m_texCoords;
    m_tangents = m.m_tangents;
    m_bitangents = m.m_bitangents;

    m_indices = m.m_indices;
    m_groups = m.m_groups;

    m_meshPath = m.m_meshPath;
    m_format = m.m_format;

    m_numVertices = m.m_numVertices;
    m_numTriangles = m.m_numTriangles;
    m_hasPositions = m.m_hasPositions;
    m_hasNormals = m.m_hasNormals;
    m_hasTexCoords = m.m_hasTexCoords;
    m_hasTangents = m.m_hasTangents;
    m_hasBitangents = m.m_hasBitangents;
    m_width = m.m_width;
    m_height = m.m_height;
    m_length = m.m_length;
    m_radius = m.m_radius;

    m_center[0] = m.m_center[0];
    m_center[1] = m.m_center[1];
    m_center[2] = m.m_center[2];

    m_extent[0] = m.m_extent[0];
    m_extent[1] = m.m_extent[1];
    m_extent[2] = m.m_extent[2];

}

Mesh::~Mesh()
{
    destroy();
}

Mesh& Mesh::operator=(const Mesh& m)
{
    if(this != &m)
    {
        destroy();

        m_positions = m.m_positions;
        m_normals = m.m_normals;
        m_texCoords = m.m_texCoords;
        m_tangents = m.m_tangents;
        m_bitangents = m.m_bitangents;

        m_indices = m.m_indices;
        m_groups = m.m_groups;

        m_meshPath = m.m_meshPath;
        m_format = m.m_format;

        m_numVertices = m.m_numVertices;
        m_numTriangles = m.m_numTriangles;
        m_hasPositions = m.m_hasPositions;
        m_hasNormals = m.m_hasNormals;
        m_hasTexCoords = m.m_hasTexCoords;
        m_hasTangents = m.m_hasTangents;
        m_hasBitangents = m.m_hasBitangents;
        m_width = m.m_width;
        m_height = m.m_height;
        m_length = m.m_length;
        m_radius = m.m_radius;

        m_center[0] = m.m_center[0];
        m_center[1] = m.m_center[1];
        m_center[2] = m.m_center[2];

        m_extent[0] = m.m_extent[0];
        m_extent[1] = m.m_extent[1];
        m_extent[2] = m.m_extent[2];
    }
    return *this;
}

void Mesh::bounds(float center[3], float &width, float &height,
                      float &length, float &radius, float extent[3]) const
{
    float xMax = std::numeric_limits<float>::min();
    float yMax = std::numeric_limits<float>::min();
    float zMax = std::numeric_limits<float>::min();

    float xMin = std::numeric_limits<float>::max();
    float yMin = std::numeric_limits<float>::max();
    float zMin = std::numeric_limits<float>::max();

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    int numVerts = static_cast<int>(m_positions.size());

    for (int i = 0; i < numVerts; ++i)
    {
        x = m_positions[i + 0];
        y = m_positions[i + 1];
        z = m_positions[i + 2];

        if (x < xMin)
            xMin = x;

        if (x > xMax)
            xMax = x;

        if (y < yMin)
            yMin = y;

        if (y > yMax)
            yMax = y;

        if (z < zMin)
            zMin = z;

        if (z > zMax)
            zMax = z;
    }

    center[0] = (xMin + xMax) / 2.0f;
    center[1] = (yMin + yMax) / 2.0f;
    center[2] = (zMin + zMax) / 2.0f;

    width = xMax - xMin;
    height = yMax - yMin;
    length = zMax - zMin;

    extent[0] = width/2.0f;
    extent[1] = height/2.0f;
    extent[2] = length/2.0f;
    
//    radius = std::max(std::max(width, height), length);
    radius = sqrtf(extent[0]*extent[0] + extent[1]*extent[1] + extent[2]*extent[2]);
}
void Mesh::setIndexFormat(const char* format)
{
    m_format.indexType = format;
}

void Mesh::addAttribute(const char* attrName, int attrSize, const char* attrType)
{
    if(containsAttribute(attrName) == false)
    {
        m_format.attributeName.push_back(attrName);
        m_format.attributeSize.push_back(attrSize);
        m_format.attributeType.push_back(attrType);
        ++m_format.attributeCount;
        
    }
}

void Mesh::removeAttribute(const char* attributeName)
{
    std::vector<unsigned int> idx;
    
    for(unsigned int i = 0; i < m_format.attributeName.size(); ++i)
    {
        if(m_format.attributeName[i].compare(attributeName)==0)
        {
            idx.push_back(i);
        }
    }
    for(std::vector<unsigned int>::iterator it = idx.begin(); it != idx.end(); ++it)
    {
        m_format.attributeName.erase(m_format.attributeName.begin()+(*it));
        m_format.attributeSize.erase(m_format.attributeSize.begin()+(*it));
        m_format.attributeType.erase(m_format.attributeType.begin()+(*it));
        --m_format.attributeCount;
    }
    
}


bool Mesh::containsAttribute(const char* attrName)
{
    for(unsigned int i = 0; i < m_format.attributeName.size(); ++i)
    {
        if(m_format.attributeName[i].compare(attrName)== 0)
            return true;
    }
    return false;
}

int Mesh::getAttributeIndexWithName(const char* attrName)
{
    for(unsigned int i = 0; i < m_format.attributeName.size(); ++i)
    {
        if(m_format.attributeName[i].compare(attrName)== 0)
            return i;
    }
    return -1;
}

int Mesh::getGroupIndexWithName(const char *groupName)
{
    for(unsigned int i = 0; i < m_groups.size(); ++i)
    {
        if(std::string(m_groups[i].name).compare(groupName) == 0)
            return i;
    }
    return -1;

}

void Mesh::printData()
{
    std::cout << "Number of Vertices: " << m_positions.size() << std::endl;
    std::cout << "Positions:" << std::endl;
    int counter = 0;
    for(std::vector<float>::iterator it = m_positions.begin(); it < m_positions.end(); ++it)
    {
        std::cout << *it << " ";
        ++counter;
        if(counter == 3)
            std::cout << std::endl; counter = 0;
    }
    std::cout << "Normals:" << std::endl;
    counter = 0;
    for(std::vector<float>::iterator it = m_normals.begin(); it < m_normals.end(); ++it)
    {
        std::cout << *it << " ";
        ++counter;
        if(counter == 3)
            std::cout << std::endl; counter = 0;
//        std::cout << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << std::endl;
    }
    std::cout << "TexCoords:" << std::endl;
    counter = 0;
    for(std::vector<float>::iterator it = m_texCoords.begin(); it < m_texCoords.end(); ++it)
    {
        std::cout << *it << " ";
        ++counter;
        if(counter == 2)
            std::cout << std::endl; counter = 0;
    }
    
    std::cout << "Indices:" << std::endl;
    for(std::vector<unsigned int>::iterator it2 = m_indices.begin(); it2 < m_indices.end(); ++it2)
    {
        std::cout << *it2 << std::endl;
    }
    
}
void Mesh::destroy()
{
    m_numVertices = 0;
    m_numTriangles = 0;

    m_hasPositions = false;
    m_hasNormals = false;
    m_hasTexCoords = false;
    m_hasTangents = false;
    m_hasBitangents = false;

    m_center[0] = 0.0f;
    m_center[1] = 0.0f;
    m_center[2] = 0.0f;
    m_width = 0.0f;
    m_height = 0.0f;
    m_length = 0.0f;
    m_radius = 0.0f;
    m_extent[0] = 0.0f;
    m_extent[1] = 0.0f;
    m_extent[2] = 0.0f;

    m_groups.clear();
    m_indices.clear();

    m_positions.clear();
    m_normals.clear();
    m_texCoords.clear();
    m_tangents.clear();
    m_bitangents.clear();

    m_format.name = "Assembly3D.mesh";
    m_format.attributeName.clear();
    m_format.attributeSize.clear();
    m_format.attributeType.clear();
    m_format.indexType = "UNSIGNED_INT";
    m_format.attributeCount = 0;
    m_format.isBinary = false;
    
}

void Mesh::calculateBounds()
{
    m_center[0] = 0.0f;
    m_center[1] = 0.0f;
    m_center[2] = 0.0f;
    m_width = 0.0f;
    m_height = 0.0f;
    m_length = 0.0f;
    m_radius = 0.0f;
    m_extent[0] = 0.0f;
    m_extent[1] = 0.0f;
    m_extent[2] = 0.0f;
    
    bounds(m_center, m_width, m_height, m_length, m_radius, m_extent);
}
void Mesh::setMeshPath(const char* path)
{
    m_meshPath = path;
}
void Mesh::hasPositions(bool val)
{
    m_hasPositions = val;
}
void Mesh::hasNormals(bool val)
{
    m_hasNormals = val;
}
void Mesh::hasTexCoords(bool val)
{
    m_hasTexCoords = val;
}
void Mesh::hasTangents(bool val)
{
    m_hasTangents = val;
}
void Mesh::hasBitangents(bool val)
{
    m_hasBitangents = val;
}

void Mesh::setPositions(const std::vector<float>& positions)
{
    m_positions = positions;
}
void Mesh::setNormals(const std::vector<float>& normals)
{
    m_normals = normals;
}
void Mesh::setTexCoords(const std::vector<float>& texCoords)
{
    m_texCoords = texCoords;
}
void Mesh::setTangents(const std::vector<float>& tangents)
{
    m_tangents = tangents;
}
void Mesh::setBitangents(const std::vector<float>& bitangents)
{
    m_bitangents = bitangents;
}
void Mesh::addPosition(float* position, int size)
{
    for(int i = 0; i < size; ++i)
    {
        m_positions.push_back(position[i]);
    }
}
void Mesh::addNormal(float* normal, int size)
{
    for(int i = 0; i < size; ++i)
    {
        m_normals.push_back(normal[i]);
    }
}
void Mesh::addTexCoord(float* texCoord, int size)
{
    for(int i = 0; i < size; ++i)
    {
        m_texCoords.push_back(texCoord[i]);
    }
}
void Mesh::addTangent(float* tangent, int size)
{
    for(int i = 0; i < size; ++i)
    {
        m_tangents.push_back(tangent[i]);
    }
}
void Mesh::addBitangent(float* bitangent, int size)
{
    for(int i = 0; i < size; ++i)
    {
        m_bitangents.push_back(bitangent[i]);
    }
}

void Mesh::clearVertices()
{
//    m_vertices.clear();
    m_positions.clear();
    m_normals.clear();
    m_texCoords.clear();
    m_tangents.clear();
    m_bitangents.clear();
}
void Mesh::addIndex(unsigned int index)
{
    m_indices.push_back(index);
}
void Mesh::clearIndices()
{
    m_indices.clear();
}
void Mesh::addGroup(Group group)
{
    m_groups.push_back(group);
}

void Mesh::setNumVertices(int numVertices)
{
    m_numVertices = numVertices;
}

void Mesh::setNumTriangles(int numTriangles)
{
    m_numTriangles = numTriangles;
}

void Mesh::initializeMeshFormat()
{
    m_format.name = "Assembly3D.mesh";
    m_format.isBinary = true;

    if(getNumberOfVertices() < (1 << 8))
        m_format.indexType = "UNSIGNED_BYTE";
    else if(getNumberOfVertices() < (1 << 16))
        m_format.indexType = "UNSIGNED_SHORT";
    else
        m_format.indexType = "UNSIGNED_INT";

    m_format.attributeCount = 0;
    if(m_hasPositions)
    {
        ++m_format.attributeCount;
        m_format.attributeName.push_back("POSITION");
        m_format.attributeSize.push_back(3);
        m_format.attributeType.push_back("FLOAT");
    }
    if(m_hasNormals)
    {
        ++m_format.attributeCount;
        m_format.attributeName.push_back("NORMAL");
        m_format.attributeSize.push_back(3);
        m_format.attributeType.push_back("FLOAT");
    }
    if(m_hasTexCoords)
    {
        ++m_format.attributeCount;
        m_format.attributeName.push_back("TEXCOORD");
        m_format.attributeSize.push_back(2);
        m_format.attributeType.push_back("FLOAT");
    }
    if(m_hasTangents)
    {
        ++m_format.attributeCount;
        m_format.attributeName.push_back("TANGENT");
        m_format.attributeSize.push_back(3);
        m_format.attributeType.push_back("FLOAT");
    }
    if(m_hasBitangents)
    {
        ++m_format.attributeCount;
        m_format.attributeName.push_back("BITANGENT");
        m_format.attributeSize.push_back(3);
        m_format.attributeType.push_back("FLOAT");
    }

}

void Mesh::generateNormals()
{
    const unsigned int *pTriangle = 0;
    float edge1[3] = {0.0f, 0.0f, 0.0f};
    float edge2[3] = {0.0f, 0.0f, 0.0f};
    float normal[3] = {0.0f, 0.0f, 0.0f};
    float length = 0.0f;
    int totalVertices = this->getNumberOfVertices();
    int totalTriangles = this->getNumberOfTriangles();

    // Initialize all the vertex normals.
    m_normals.clear();
    m_normals.assign(totalVertices*3, 0.0f);

    // Calculate the vertex normals.
    for (int i = 0; i < totalTriangles; ++i)
    {
        pTriangle = this->getTriangle(i);
        float* pPosition0 = getPosition(pTriangle[0]);
        float* pPosition1 = getPosition(pTriangle[1]);
        float* pPosition2 = getPosition(pTriangle[2]);

        // Calculate triangle face normal.

        edge1[0] = pPosition1[0] - pPosition0[0];
        edge1[1] = pPosition1[1] - pPosition0[1];
        edge1[2] = pPosition1[2] - pPosition0[2];

        edge2[0] = pPosition2[0] - pPosition0[0];
        edge2[1] = pPosition2[1] - pPosition0[1];
        edge2[2] = pPosition2[2] - pPosition0[2];

        normal[0] = (edge1[1] * edge2[2]) - (edge1[2] * edge2[1]);
        normal[1] = (edge1[2] * edge2[0]) - (edge1[0] * edge2[2]);
        normal[2] = (edge1[0] * edge2[1]) - (edge1[1] * edge2[0]);

        // normalizing
        length = 0.0f;
        float tmpLength = 0.0f;
        tmpLength = sqrtf(normal[0] * normal[0] +
            normal[1] * normal[1] +
            normal[2] * normal[2]);

        if(tmpLength == 0.0f)
            length = 0.0f;
        else
            length = 1.0f / tmpLength;

        normal[0] *= length;
        normal[1] *= length;
        normal[2] *= length;


        // Accumulate the normals.
        float* pNormal0 = getNormal(pTriangle[0]);
        float* pNormal1 = getNormal(pTriangle[1]);;
        float* pNormal2 = getNormal(pTriangle[2]);;

        pNormal0[0] += normal[0];
        pNormal0[1] += normal[1];
        pNormal0[2] += normal[2];

        pNormal1[0] += normal[0];
        pNormal1[1] += normal[1];
        pNormal1[2] += normal[2];

        pNormal2[0] += normal[0];
        pNormal2[1] += normal[1];
        pNormal2[2] += normal[2];

    }

    // Normalize the vertex normals.
    for (int i = 0; i < totalVertices; ++i)
    {
//        pVertex0 = &this->getVertex(i);
        float* pNormal = getNormal(i);

        float tmpLength = sqrtf(pNormal[0] * pNormal[0] +
            pNormal[1] * pNormal[1] + pNormal[2] * pNormal[2]);

        if(tmpLength == 0.0f)
            length = 0.0f;
        else
            length = 1.0f / tmpLength;

        pNormal[0] *= length;
        pNormal[1] *= length;
        pNormal[2] *= length;
    }

    //this->printData();
    this->hasNormals(true);
//    updateVecs();
}

void Mesh::generateTangents()
{
    const unsigned int *pTriangle = 0;
//    Vertex *pVertex0 = 0;
//    Vertex *pVertex1 = 0;
//    Vertex *pVertex2 = 0;
    float edge1[3] = {0.0f, 0.0f, 0.0f};
    float edge2[3] = {0.0f, 0.0f, 0.0f};
    float texEdge1[2] = {0.0f, 0.0f};
    float texEdge2[2] = {0.0f, 0.0f};
    float tangent[3] = {0.0f, 0.0f, 0.0f};
    float bitangent[3] = {0.0f, 0.0f, 0.0f};
    float det = 0.0f;
    float nDotT = 0.0f;
    float bDotB = 0.0f;
    float length = 0.0f;
    int totalVertices = this->getNumberOfVertices();
    int totalTriangles = this->getNumberOfTriangles();

    // Initialize all the vertex tangents and bitangents.
    m_tangents.clear();
    m_bitangents.clear();
    m_tangents.assign(totalVertices*3, 0.0f);
    m_bitangents.assign(totalVertices*3, 0.0f);

    // Calculate the vertex tangents and bitangents.
    for (int i = 0; i < totalTriangles; ++i)
    {
        pTriangle = this->getTriangle(i);
//        pVertex0 = &this->getVertex(pTriangle[0]);
//        pVertex1 = &this->getVertex(pTriangle[1]);
//        pVertex2 = &this->getVertex(pTriangle[2]);

        // Calculate the triangle face tangent and bitangent.
        float* pPosition0 = getPosition(pTriangle[0]);
        float* pPosition1 = getPosition(pTriangle[1]);
        float* pPosition2 = getPosition(pTriangle[2]);

        edge1[0] = pPosition1[0] - pPosition0[0];
        edge1[1] = pPosition1[1] - pPosition0[1];
        edge1[2] = pPosition1[2] - pPosition0[2];

        edge2[0] = pPosition2[0] - pPosition0[0];
        edge2[1] = pPosition2[1] - pPosition0[1];
        edge2[2] = pPosition2[2] - pPosition0[2];

        float* pTexCoord0 = getTexCoord(pTriangle[0]);
        float* pTexCoord1 = getTexCoord(pTriangle[1]);
        float* pTexCoord2 = getTexCoord(pTriangle[2]);

        texEdge1[0] = pTexCoord1[0] - pTexCoord0[0];
        texEdge1[1] = pTexCoord1[1] - pTexCoord0[1];

        texEdge2[0] = pTexCoord2[0] - pTexCoord0[0];
        texEdge2[1] = pTexCoord2[1] - pTexCoord0[1];

        det = texEdge1[0] * texEdge2[1] - texEdge2[0] * texEdge1[1];

        if (fabs(det) < 1e-6f)
        {
            tangent[0] = 1.0f;
            tangent[1] = 0.0f;
            tangent[2] = 0.0f;

            bitangent[0] = 0.0f;
            bitangent[1] = 1.0f;
            bitangent[2] = 0.0f;
        }
        else
        {
            det = 1.0f / det;

            tangent[0] = (texEdge2[1] * edge1[0] - texEdge1[1] * edge2[0]) * det;
            tangent[1] = (texEdge2[1] * edge1[1] - texEdge1[1] * edge2[1]) * det;
            tangent[2] = (texEdge2[1] * edge1[2] - texEdge1[1] * edge2[2]) * det;

            bitangent[0] = (-texEdge2[0] * edge1[0] + texEdge1[0] * edge2[0]) * det;
            bitangent[1] = (-texEdge2[0] * edge1[1] + texEdge1[0] * edge2[1]) * det;
            bitangent[2] = (-texEdge2[0] * edge1[2] + texEdge1[0] * edge2[2]) * det;
        }

        // Accumulate the tangents and bitangents.

        float* pTangent0 = getTangent(pTriangle[0]);
        float* pTangent1= getTangent(pTriangle[1]);;
        float* pTangent2 = getTangent(pTriangle[2]);;
        float* pBitangent0 = getBitangent(pTriangle[0]);
        float* pBitangent1= getBitangent(pTriangle[1]);;
        float* pBitangent2 = getBitangent(pTriangle[2]);;

        pTangent0[0] += tangent[0];
        pTangent0[1] += tangent[1];
        pTangent0[2] += tangent[2];
        pBitangent0[0] += bitangent[0];
        pBitangent0[1] += bitangent[1];
        pBitangent0[2] += bitangent[2];

        pTangent1[0] += tangent[0];
        pTangent1[1] += tangent[1];
        pTangent1[2] += tangent[2];
        pBitangent1[0] += bitangent[0];
        pBitangent1[1] += bitangent[1];
        pBitangent1[2] += bitangent[2];

        pTangent2[0] += tangent[0];
        pTangent2[1] += tangent[1];
        pTangent2[2] += tangent[2];
        pBitangent2[0] += bitangent[0];
        pBitangent2[1] += bitangent[1];
        pBitangent2[2] += bitangent[2];
    }

    // Orthogonalize and normalize the vertex tangents.
    for (int i = 0; i < totalVertices; ++i)
    {
        float* pNormal = getNormal(i);
        float* pTangent = getTangent(i);

        // Gram-Schmidt orthogonalize tangent with normal.

        nDotT = pNormal[0] * pTangent[0] +
                pNormal[1] * pTangent[1] +
                pNormal[2] * pTangent[2];

        pTangent[0] -= pNormal[0] * nDotT;
        pTangent[1] -= pNormal[1] * nDotT;
        pTangent[2] -= pNormal[2] * nDotT;

        // Normalize the tangent.

        length = 1.0f / sqrtf(pTangent[0] * pTangent[0] +
                              pTangent[1] * pTangent[1] +
                              pTangent[2] * pTangent[2]);

        pTangent[0] *= length;
        pTangent[1] *= length;
        pTangent[2] *= length;

        // Calculate the handedness of the local tangent space.
        // The bitangent vector is the cross product between the triangle face
        // normal vector and the calculated tangent vector. The resulting
        // bitangent vector should be the same as the bitangent vector
        // calculated from the set of linear equations above. If they point in
        // different directions then we need to invert the cross product
        // calculated bitangent vector. We store this scalar multiplier in the
        // tangent vector's 'w' component so that the correct bitangent vector
        // can be generated in the normal mapping shader's vertex shader.
        //
        // Normal maps have a left handed coordinate system with the origin
        // located at the top left of the normal map texture. The x coordinates
        // run horizontally from left to right. The y coordinates run
        // vertically from top to bottom. The z coordinates run out of the
        // normal map texture towards the viewer. Our handedness calculations
        // must take this fact into account as well so that the normal mapping
        // shader's vertex shader will generate the correct bitangent vectors.
        // Some normal map authoring tools such as Crazybump
        // (http://www.crazybump.com/) includes options to allow you to control
        // the orientation of the normal map normal's y-axis.

        bitangent[0] = (pNormal[1] * pTangent[2]) -
                       (pNormal[2] * pTangent[1]);
        bitangent[1] = (pNormal[2] * pTangent[0]) -
                       (pNormal[0] * pTangent[2]);
        bitangent[2] = (pNormal[0] * pTangent[1]) -
                       (pNormal[1] * pTangent[0]);

        float* pBitangent = getBitangent(i);
        bDotB = bitangent[0] * pBitangent[0] +
                bitangent[1] * pBitangent[1] +
                bitangent[2] * pBitangent[2];

//        pVertex0->tangent[3] = (bDotB < 0.0f) ? 1.0f : -1.0f;

        pBitangent[0] = bitangent[0];
        pBitangent[1] = bitangent[1];
        pBitangent[2] = bitangent[2];
    }

    this->hasTangents(true);
    this->hasBitangents(true);
}

//void Mesh::setAttributes()
//{
//    m_attributes.clear();
//    Mesh::Attribute position;
//    position.data = getPositionsPointer();
//    position.count = static_cast<int>(m_positions.size());
//    position.size = 3;
//    m_attributes.push_back(position);
//    Mesh::Attribute normal;
//    normal.data = getNormalsPointer();
//    normal.count = static_cast<int>(m_normals.size());
//    normal.size = 3;
//    m_attributes.push_back(normal);
//    Mesh::Attribute texCoord;
//    texCoord.data = getTexCoordsPointer();
//    texCoord.count = static_cast<int>(m_texCoords.size());
//    texCoord.size = 2;
//    m_attributes.push_back(texCoord);
//    Mesh::Attribute tangents;
//    tangents.data = getTangentsPointer();
//    tangents.count = static_cast<int>(m_tangents.size());
//    tangents.size = 3;
//    m_attributes.push_back(tangents);
//    Mesh::Attribute bitangents;
//    bitangents.data = getBitangentsPointer();
//    bitangents.count = static_cast<int>(m_bitangents.size());
//    bitangents.size = 3;
//    m_attributes.push_back(bitangents);

//}

Mesh::Attribute Mesh::getAttribute(Mesh::AttributeType type)
{
    Mesh::Attribute attribute;
	int idx = -1;
    switch(type)
    {
		case Mesh::POSITION:
			attribute.data = getPositionsPointer();
			attribute.count = static_cast<int>(m_positions.size());
			idx = getAttributeIndexWithName("POSITION");
			attribute.size = m_format.attributeSize[idx];
			attribute.type = Mesh::POSITION;
			break;
		case Mesh::NORMAL:
			attribute.data = getNormalsPointer();
			attribute.count = static_cast<int>(m_normals.size());
			idx = getAttributeIndexWithName("NORMAL");
			attribute.size = m_format.attributeSize[idx];
			attribute.type = Mesh::NORMAL;
			break;
		case Mesh::TEXCOORD:
			attribute.data = getTexCoordsPointer();
			attribute.count = static_cast<int>(m_texCoords.size());
			idx = getAttributeIndexWithName("TEXCOORD");
			attribute.size = m_format.attributeSize[idx];
			attribute.type = Mesh::TEXCOORD;
			break;
		case Mesh::TANGENT:
			attribute.data = getTangentsPointer();
			attribute.count = static_cast<int>(m_tangents.size());
			idx = getAttributeIndexWithName("TANGENT");
			attribute.size = m_format.attributeSize[idx];
			attribute.type = Mesh::TANGENT;
			break;
		case Mesh::BITANGENT:
			attribute.data = getBitangentsPointer();
			attribute.count = static_cast<int>(m_bitangents.size());
			idx = getAttributeIndexWithName("BITANGENT");
			attribute.size = m_format.attributeSize[idx];
			attribute.type = Mesh::BITANGENT;
			break;
		default:
			attribute.data = 0;
			attribute.count = 0;
			attribute.size = 0;
			break;
    }

    return attribute;
}

void assembly3d::Mesh::printAttribute(assembly3d::Mesh::Attribute a)
{
    for(int i = 0; i < a.count/a.size; ++i){
        for(int j = 0; j < a.size; ++j)
        {
            std::cout << a.data[i*a.size + j];
        }
        std::cout << std::endl;
    }
}

//void Mesh::addTriangle(unsigned int *triangle)
//{
//}

//void Mesh::updateVecs()
//{
//    m_positions.clear();
//    m_normals.clear();
//    m_texCoords.clear();

//    using namespace std;
//    for(size_t idx = 0; idx < m_vertices.size(); ++idx)
//    {
//        Vertex* vert = &m_vertices[idx];
//        m_positions.push_back(vert->position[0]);
//        m_positions.push_back(vert->position[1]);
//        m_positions.push_back(vert->position[2]);
//        m_normals.push_back(vert->normal[0]);
//        m_normals.push_back(vert->normal[1]);
//        m_normals.push_back(vert->normal[2]);
//        m_texCoords.push_back(vert->texCoord[0]);
//        m_texCoords.push_back(vert->texCoord[1]);
//    }
//}
