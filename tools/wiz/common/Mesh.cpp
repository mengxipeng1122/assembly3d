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
    m_faceNormals = m.m_faceNormals;

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
        m_faceNormals = m.m_faceNormals;

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

    for (int i = 0; i < numVerts/4; ++i)
    {
        x = m_positions[i*4 + 0];
        y = m_positions[i*4 + 1];
        z = m_positions[i*4 + 2];

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
    m_faceNormals.clear();

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
    m_faceNormals.clear();
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

float* Mesh::getFaceNormals()
{
    const unsigned int *pTriangle = 0;
    float edge1[3] = {0.0f, 0.0f, 0.0f};
    float edge2[3] = {0.0f, 0.0f, 0.0f};
    float normal[3] = {0.0f, 0.0f, 0.0f};
    float length = 0.0f;
    int totalVertices = this->getNumberOfVertices();
    int totalTriangles = this->getNumberOfTriangles();

    // Initialize all the vertex normals.
    m_faceNormals.clear();
    m_faceNormals.assign(totalVertices*4, 0.0f);
	
	

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
        float* pNormal0 = &m_faceNormals[pTriangle[0]*4];
        float* pNormal1 = &m_faceNormals[pTriangle[1]*4];
        float* pNormal2 = &m_faceNormals[pTriangle[2]*4];

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
        float* pNormal = &m_faceNormals[i*4];

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
	
	return &m_faceNormals[0];
}

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
