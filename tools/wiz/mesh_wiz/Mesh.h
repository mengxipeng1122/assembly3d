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

#ifndef _MESH_H_
#define _MESH_H_

#include "MeshWizIncludes.h"
#include <vector>
#include <iostream>
using namespace MeshWiz;

class Mesh
{
public:
    struct MeshFormat
    {
        std::string name;
        int attributeCount;
        std::vector<std::string> attributeName;
        std::vector<int> attributeSize;
        std::vector<std::string> attributeType;
        std::string indexType;
        bool isBinary;
    };

	Mesh();
	~Mesh();
    
    void destroy();
    
    void printData();
    
    void setIndexFormat(const std::string& format);
    
    Vertex& getVertex(unsigned int index);
	Group& getGroup(unsigned int index);
	unsigned int* getTriangle(unsigned int index);
    
    int getNumberOfTriangles() const { return m_numTriangles; }
    unsigned int getNumberOfVertices() const { return m_vertices.size(); }
	unsigned int getNumberOfGroups() const { return m_groups.size(); }
    
    void hasPositions(bool val) { m_hasPositions = val; }
    void hasNormals(bool val) { m_hasNormals = val; }
    void hasTexCoords(bool val) { m_hasTexCoords = val; }
    void hasTangents(bool val) { m_hasTangents = val; }
    void hasBitangents(bool val) { m_hasBitangents = val; }
    
    bool hasPositions() const { return m_hasPositions; }
    bool hasNormals() const { return m_hasNormals; }
    bool hasTexCoords() const { return m_hasTexCoords; }
    bool hasTangents() const { return m_hasTangents; }
    bool hasBitangents() const { return m_hasBitangents; }
    
    MeshFormat& getMeshFormat() { return m_format; }
    void addVertex(Vertex vertex) { m_vertices.push_back(vertex); }
    void addIndex(unsigned int index) { m_indices.push_back(index); }
    void addGroup(Group group) { m_groups.push_back(group); }
    void setNumTriangles(int numTriangles) { m_numTriangles = numTriangles; }
    
    
    void addAttribute(const std::string& name, int attrSize, const std::string& attrType);
    void removeAttribute(const std::string& attributeName);
    bool containsAttribute(const std::string& attrName);
    int getAttributeIndexWithName(const std::string& attrName);
    
//    void normalize(float scaleTo = 1.0f, bool center = true);
//    void reverseWinding();

    // Getter methods.

    void getCenter(float &x, float &y, float &z) const;
    float getWidth() const;
    float getHeight() const;
    float getLength() const;
    float getRadius() const;
    void getExtent(float &x, float &y, float &z) const;
    
    void calculateBounds();
    
    void setMeshPath(const std::string& path) { m_MeshPath = path; }
private:

    void bounds(float center[3], float &width, float &height,
    float &length, float &radius, float extent[3]) const;
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Group> m_groups;
    
    MeshFormat m_format;
    
    std::string m_MeshPath;
    
    int m_numTriangles;
    
    bool m_hasPositions;
    bool m_hasNormals;
    bool m_hasTexCoords;
    bool m_hasTangents;
    bool m_hasBitangents;
    
    float m_center[3];
    float m_width;
    float m_height;
    float m_length;
    float m_radius;
    float m_extent[3];
    
    friend std::ostream& operator<<(std::ostream& os, Mesh& obj);
};

inline std::ostream& operator<<(std::ostream& os, Mesh& obj)
{
    os << "Assebmbly3D mesh info: \n---------------------------\n";
    os << "Format:      " << (obj.getMeshFormat().isBinary == true ? "binary" : "debug") << "\n";
	os << "Vertices:    " << obj.getNumberOfVertices() << "\n";
    os << "Index type:  " << obj.getMeshFormat().indexType << "\n";
    os << "Positions:   " << (obj.hasPositions() == true ? "yes" : "no" ) << "\n";
    os << "Normals:     " << (obj.hasNormals() == true ? "yes" : "no" ) << "\n";
    os << "TexCoords:   " << (obj.hasTexCoords() == true ? "yes" : "no" ) << "\n";
    os << "Tangents:    " << (obj.hasTangents() == true ? "yes" : "no" ) << "\n";
    os << "Bitangents:  " << (obj.hasBitangents() == true ? "yes" : "no" ) << "\n";
    os << "Groups:      " << obj.getNumberOfGroups() << "\n";
    os << "Triangles:   " << obj.getNumberOfTriangles() << "\n";
    os << "Index type:  " << obj.getMeshFormat().indexType << "\n";
    os << "---------------------------\n";
    os << "Width:       " << obj.getWidth() << "\n";
    os << "Height:      " << obj.getHeight() << "\n";
    os << "Length:      " << obj.getLength() << "\n";
    float x, y, z;
    obj.getCenter(x, y, z);
    os << "Center:      " << x << " / " << y << " / " << z << "\n";
    x = y = z = 0.0f;
    obj.getExtent(x, y, z);
    os << "Extent:      " << x << " / " << y << " / " << z << "\n";
    os << "Radius:      " << obj.getRadius() << "\n";
//    os << "Format: ";
	return os;
}

inline void Mesh::getCenter(float &x, float &y, float &z) const
{ x = m_center[0]; y = m_center[1]; z = m_center[2]; }

inline float Mesh::getWidth() const
{ return m_width; }

inline float Mesh::getHeight() const
{ return m_height; }

inline float Mesh::getLength() const
{ return m_length; }

inline float Mesh::getRadius() const
{ return m_radius; }

inline void Mesh::getExtent(float &x, float &y, float &z) const
{ x = m_extent[0]; y = m_extent[1]; z = m_extent[2]; }

inline Vertex& Mesh::getVertex(unsigned int index)
{ return m_vertices[index]; }

inline Group& Mesh::getGroup(unsigned int index)
{ return m_groups[index]; }

inline unsigned int* Mesh::getTriangle(unsigned int index)
{ return &m_indices[index * 3]; }

#endif  // _MESH_H_
