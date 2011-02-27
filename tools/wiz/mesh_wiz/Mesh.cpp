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

using namespace MeshWiz;

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
void Mesh::setIndexFormat(const std::string& format)
{
    m_format.indexType = format;
}

void Mesh::addAttribute(const std::string& attrName, int attrSize, const std::string& attrType)
{
    if(!containsAttribute(attrName))
    {
        m_format.attributeName.push_back(attrName);
        m_format.attributeSize.push_back(attrSize);
        m_format.attributeType.push_back(attrType);
        ++m_format.attributeCount;
        
    }
}

void Mesh::removeAttribute(const std::string& attributeName)
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


bool Mesh::containsAttribute(const std::string& attrName)
{
    for(unsigned int i = 0; i < m_format.attributeName.size(); ++i)
    {
        if(m_format.attributeName[i].compare(attrName)== 0)
            return true;
    }
    return false;
}

int Mesh::getAttributeIndexWithName(const std::string& attrName)
{
    for(unsigned int i = 0; i < m_format.attributeName.size(); ++i)
    {
        if(m_format.attributeName[i].compare(attrName)== 0)
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
    
    m_groups.clear();
    m_vertices.clear();
    m_indices.clear();
    
    m_format.name = "Assembly3D";
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
