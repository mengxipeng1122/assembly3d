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
#include <limits>
#include <cmath>
#include <algorithm>

using namespace assembly3d;

Mesh::Mesh()
:
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

Mesh::~Mesh()
{
    
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

    int numVerts = static_cast<int>(m_vertices.size());

    for (int i = 0; i < numVerts; ++i)
    {
        x = m_vertices[i].position[0];
        y = m_vertices[i].position[1];
        z = m_vertices[i].position[2];

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
    std::cout << "Number of Vertices: " << m_vertices.size() << std::endl;
    std::cout << "Positions:" << std::endl;
    for(std::vector<Vertex>::iterator it = m_vertices.begin(); it < m_vertices.end(); ++it)
    {
        std::cout << it->position[0] << " " << it->position[1] << " " << it->position[2] << std::endl;
    }
    std::cout << "Normals:" << std::endl;
    for(std::vector<Vertex>::iterator it = m_vertices.begin(); it < m_vertices.end(); ++it)
    {
        std::cout << it->normal[0] << " " << it->normal[1] << " " << it->normal[2] << std::endl;
    }
    std::cout << "TexCoords:" << std::endl;
    for(std::vector<Vertex>::iterator it = m_vertices.begin(); it < m_vertices.end(); ++it)
    {
        std::cout << it->texCoord[0] << " " << it->texCoord[1] << std::endl;
    }
    
    std::cout << "Indices:" << std::endl;
    for(std::vector<unsigned int>::iterator it2 = m_indices.begin(); it2 < m_indices.end(); ++it2)
    {
        std::cout << *it2 << std::endl;
    }
    
}
void Mesh::destroy()
{
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
    m_vertices.clear();
    m_indices.clear();
    
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
void Mesh::addVertex(Vertex vertex)
{
    m_vertices.push_back(vertex);
}
void Mesh::clearVertices()
{
    m_vertices.clear();
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
        m_format.attributeName.push_back("TEXTURE");
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
    Vertex *pVertex0 = 0;
    Vertex *pVertex1 = 0;
    Vertex *pVertex2 = 0;
    float edge1[3] = {0.0f, 0.0f, 0.0f};
    float edge2[3] = {0.0f, 0.0f, 0.0f};
    float normal[3] = {0.0f, 0.0f, 0.0f};
    float length = 0.0f;
    int totalVertices = this->getNumberOfVertices();
    int totalTriangles = this->getNumberOfTriangles();

    // Initialize all the vertex normals.
    for (int i = 0; i < totalVertices; ++i)
    {
        pVertex0 = &this->getVertex(i);
        pVertex0->normal[0] = 0.0f;
        pVertex0->normal[1] = 0.0f;
        pVertex0->normal[2] = 0.0f;
    }

    // Calculate the vertex normals.
    for (int i = 0; i < totalTriangles; ++i)
    {
        pTriangle = this->getTriangle(i);
        pVertex0 = &this->getVertex(pTriangle[0]);
        pVertex1 = &this->getVertex(pTriangle[1]);
        pVertex2 = &this->getVertex(pTriangle[2]);

        // Calculate triangle face normal.

        edge1[0] = pVertex1->position[0] - pVertex0->position[0];
        edge1[1] = pVertex1->position[1] - pVertex0->position[1];
        edge1[2] = pVertex1->position[2] - pVertex0->position[2];

        edge2[0] = pVertex2->position[0] - pVertex0->position[0];
        edge2[1] = pVertex2->position[1] - pVertex0->position[1];
        edge2[2] = pVertex2->position[2] - pVertex0->position[2];

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

        pVertex0->normal[0] += normal[0];
        pVertex0->normal[1] += normal[1];
        pVertex0->normal[2] += normal[2];

        pVertex1->normal[0] += normal[0];
        pVertex1->normal[1] += normal[1];
        pVertex1->normal[2] += normal[2];

        pVertex2->normal[0] += normal[0];
        pVertex2->normal[1] += normal[1];
        pVertex2->normal[2] += normal[2];
    }

    // Normalize the vertex normals.
    for (int i = 0; i < totalVertices; ++i)
    {
        pVertex0 = &this->getVertex(i);

        float tmpLength = sqrtf(pVertex0->normal[0] * pVertex0->normal[0] +
            pVertex0->normal[1] * pVertex0->normal[1] +
            pVertex0->normal[2] * pVertex0->normal[2]);

        if(tmpLength == 0.0f)
            length = 0.0f;
        else
            length = 1.0f / tmpLength;

        pVertex0->normal[0] *= length;
        pVertex0->normal[1] *= length;
        pVertex0->normal[2] *= length;
    }

    //this->printData();
    this->hasNormals(true);

}

void Mesh::generateTangents()
{
    const unsigned int *pTriangle = 0;
    Vertex *pVertex0 = 0;
    Vertex *pVertex1 = 0;
    Vertex *pVertex2 = 0;
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
    for (int i = 0; i < totalVertices; ++i)
    {
        pVertex0 = &this->getVertex(i);

        pVertex0->tangent[0] = 0.0f;
        pVertex0->tangent[1] = 0.0f;
        pVertex0->tangent[2] = 0.0f;

        pVertex0->bitangent[0] = 0.0f;
        pVertex0->bitangent[1] = 0.0f;
        pVertex0->bitangent[2] = 0.0f;
    }

    // Calculate the vertex tangents and bitangents.
    for (int i = 0; i < totalTriangles; ++i)
    {
        pTriangle = this->getTriangle(i);
        pVertex0 = &this->getVertex(pTriangle[0]);
        pVertex1 = &this->getVertex(pTriangle[1]);
        pVertex2 = &this->getVertex(pTriangle[2]);

        // Calculate the triangle face tangent and bitangent.

        edge1[0] = pVertex1->position[0] - pVertex0->position[0];
        edge1[1] = pVertex1->position[1] - pVertex0->position[1];
        edge1[2] = pVertex1->position[2] - pVertex0->position[2];

        edge2[0] = pVertex2->position[0] - pVertex0->position[0];
        edge2[1] = pVertex2->position[1] - pVertex0->position[1];
        edge2[2] = pVertex2->position[2] - pVertex0->position[2];

        texEdge1[0] = pVertex1->texCoord[0] - pVertex0->texCoord[0];
        texEdge1[1] = pVertex1->texCoord[1] - pVertex0->texCoord[1];

        texEdge2[0] = pVertex2->texCoord[0] - pVertex0->texCoord[0];
        texEdge2[1] = pVertex2->texCoord[1] - pVertex0->texCoord[1];

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

        pVertex0->tangent[0] += tangent[0];
        pVertex0->tangent[1] += tangent[1];
        pVertex0->tangent[2] += tangent[2];
        pVertex0->bitangent[0] += bitangent[0];
        pVertex0->bitangent[1] += bitangent[1];
        pVertex0->bitangent[2] += bitangent[2];

        pVertex1->tangent[0] += tangent[0];
        pVertex1->tangent[1] += tangent[1];
        pVertex1->tangent[2] += tangent[2];
        pVertex1->bitangent[0] += bitangent[0];
        pVertex1->bitangent[1] += bitangent[1];
        pVertex1->bitangent[2] += bitangent[2];

        pVertex2->tangent[0] += tangent[0];
        pVertex2->tangent[1] += tangent[1];
        pVertex2->tangent[2] += tangent[2];
        pVertex2->bitangent[0] += bitangent[0];
        pVertex2->bitangent[1] += bitangent[1];
        pVertex2->bitangent[2] += bitangent[2];
    }

    // Orthogonalize and normalize the vertex tangents.
    for (int i = 0; i < totalVertices; ++i)
    {
        pVertex0 = &this->getVertex(i);

        // Gram-Schmidt orthogonalize tangent with normal.

        nDotT = pVertex0->normal[0] * pVertex0->tangent[0] +
                pVertex0->normal[1] * pVertex0->tangent[1] +
                pVertex0->normal[2] * pVertex0->tangent[2];

        pVertex0->tangent[0] -= pVertex0->normal[0] * nDotT;
        pVertex0->tangent[1] -= pVertex0->normal[1] * nDotT;
        pVertex0->tangent[2] -= pVertex0->normal[2] * nDotT;

        // Normalize the tangent.

        length = 1.0f / sqrtf(pVertex0->tangent[0] * pVertex0->tangent[0] +
                              pVertex0->tangent[1] * pVertex0->tangent[1] +
                              pVertex0->tangent[2] * pVertex0->tangent[2]);

        pVertex0->tangent[0] *= length;
        pVertex0->tangent[1] *= length;
        pVertex0->tangent[2] *= length;

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

        bitangent[0] = (pVertex0->normal[1] * pVertex0->tangent[2]) -
                       (pVertex0->normal[2] * pVertex0->tangent[1]);
        bitangent[1] = (pVertex0->normal[2] * pVertex0->tangent[0]) -
                       (pVertex0->normal[0] * pVertex0->tangent[2]);
        bitangent[2] = (pVertex0->normal[0] * pVertex0->tangent[1]) -
                       (pVertex0->normal[1] * pVertex0->tangent[0]);

        bDotB = bitangent[0] * pVertex0->bitangent[0] +
                bitangent[1] * pVertex0->bitangent[1] +
                bitangent[2] * pVertex0->bitangent[2];

//        pVertex0->tangent[3] = (bDotB < 0.0f) ? 1.0f : -1.0f;

        pVertex0->bitangent[0] = bitangent[0];
        pVertex0->bitangent[1] = bitangent[1];
        pVertex0->bitangent[2] = bitangent[2];
    }

    this->hasTangents(true);
    this->hasBitangents(true);
}
