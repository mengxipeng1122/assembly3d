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

#include "A3DTypes.h"
#include <vector>
#include <iostream>
#include <string>

namespace assembly3d
{
    /**
     * @brief The mesh class.
     *
    */
    class Mesh
    {
    public:
        /**
         * @brief Mesh format information.
         *
        */
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
        Mesh(const Mesh& m);
        ~Mesh();

        /**
         * @brief Clears the mesh values.
         *
        */
        void destroy();

        /**
         * @brief Prints mesh datat to console.
         *
        */
        void printData();

        /**
         * @brief Sets the format of mesh indices.
         *
         * @param format A format string for indices (UNSIGNED_BYTE, UNSIGNED_SHORT or UNSIGNED_INT)
        */
        void setIndexFormat(const char* format);

        /**
         * @brief Generates normals for mesh.
         *
        */
        void generateNormals();
        /**
         * @brief Generates tangents and bitangents for mesh.
         *
        */
        void generateTangents();

        /**
         * @brief Gets a vertex for an index.
         *
         * @param index A vertex index.
         * @return Vertex at index.
        */
        assembly3d::Vertex& getVertex(unsigned int index);
        /**
         * @brief Gets a group for an index.
         *
         * @param index A group index.
         * @return Group at index.
        */
        assembly3d::Group& getGroup(unsigned int index);
        /**
         * @brief Gets a triangle for an index.
         *
         * @param index A triangle index.
         * @return triangle at index.
        */
        unsigned int* getTriangle(unsigned int index);

        /**
         * @brief Gets total number of triangles.
         *
        */
        int getNumberOfTriangles() const;
        /**
         * @brief Gets total number of vertices.
         *
        */
        unsigned int getNumberOfVertices() const;
        /**
         * @brief Gets total number of groups.
         *
        */
        unsigned int getNumberOfGroups() const;

        /**
         * @param val True if mesh has positions.
        */
        void hasPositions(bool val);
        /**
         * @param val True if mesh has normals.
        */
        void hasNormals(bool val);
        /**
         * @param val True if mesh has texture coordinates.
        */
        void hasTexCoords(bool val);
        /**
         * @param val True if mesh has tangents.
        */
        void hasTangents(bool val);
        /**
         * @param val True if mesh has bitangents
        */
        void hasBitangents(bool val);

        /**
         * @brief Returns true if mesh has positions.
         *
        */
        bool hasPositions() const;
        /**
         * @brief Returns true if mesh has normals.
         *
        */
        bool hasNormals() const;
        /**
         * @brief Returns true if mesh has texture coordinates.
         *
        */
        bool hasTexCoords() const;
        /**
         * @brief Returns true if mesh has tangents.
         *
        */
        bool hasTangents() const;
        /**
         * @brief Returns true if mesh has bitangents.
         *
        */
        bool hasBitangents() const;

        /**
         * @brief Gets the mesh format.
         *
         * @return MeshFormat &
        */
        MeshFormat& getMeshFormat();

        /**
         * @brief Adds a vertex to the mesh.
         *
         * @param vertex A vertex.
        */
        void addVertex(assembly3d::Vertex vertex);
        /**
         * @brief Clears vertices.
         *
        */
        void clearVertices();

        /**
         * @brief Adds an index to the mesh.
         *
         * @param index An index.
        */
        void addIndex(unsigned int index);
        /**
         * @brief Clears indices.
         *
        */
        void clearIndices();

        /**
         * @brief Adds a group to the mesh.
         *
         * @param group A group.
        */
        void addGroup(assembly3d::Group group);
        /**
         * @brief Sets the number of triangles.
         *
         * @param numTriangles The new number of triangles.
        */
        void setNumTriangles(int numTriangles);


        /**
         * @brief Adds an attribute.
         *
         * @param name Attribute name. (POSITION, NORMAL, TEXTURE, TANGENT and BITANGENT)
         * @param attrSize Attribute size.
         * @param attrType Attribute type. (i.e. "FLOAT")
        */
        void addAttribute(const char* name, int attrSize, const char* attrType);
        /**
         * @brief Removes an attribute.
         *
         * @param attributeName Attribute name.
        */
        void removeAttribute(const char* attributeName);
        /**
         * @brief Checks if the mesh contains a particular attribute.
         *
         * @param attrName Attribute name to check.
         * @return True if meshh contains attribute.
        */
        bool containsAttribute(const char* attrName);
        /**
         * @brief Gets the attribute's index.
         *
         * @param attrName Attribute name to get index for.
         * @return Index of the attribute or -1 if attribute not exits.
        */
        int getAttributeIndexWithName(const char* attrName);
        /**
         * @brief Gets the groups's index.
         *
         * @param groupName Group name to get index for.
         * @return Index of the group or -1 if group not exits.
        */
        int getGroupIndexWithName(const char* groupName);
    //    void normalize(float scaleTo = 1.0f, bool center = true);
    //    void reverseWinding();

        // Getter methods.

        /**
         * @brief Gets center coordinates of mesh.
         *
         * @param x X position variable of center to write in.
         * @param y Y position variable of center to write in.
         * @param z Z position variable of center to write in.
        */
        void getCenter(float &x, float &y, float &z) const;
        /**
         * @brief Gets mesh width.
         *
        */
        float getWidth() const;
        /**
         * @brief Gets mesh height.
         *
        */
        float getHeight() const;
        /**
         * @brief Gets mesh length.
         *
        */
        float getLength() const;
        /**
         * @brief Gets mesh radius.
         *
        */
        float getRadius() const;
        /**
         * @brief Gets mesh extents.
         *
         * @param x X extent variable of center to write in.
         * @param y Y extent variable of center to write in.
         * @param z Z extent variable of center to write in.
        */
        void getExtent(float &x, float &y, float &z) const;

        /**
         * @brief Calculates bounds for the mesh.
         *
        */
        void calculateBounds();

        /**
         * @brief Sets path of mesh file.
         *
         * @param path Mesh path string.
        */
        void setMeshPath(const char* path);
        /**
         * @brief Gets mesh path.
         *
         * @return const char *
        */
        const char* getMeshPath() const;

        /**
         * @brief Initilizes the mesh format.
         *
        */
        void initializeMeshFormat();

        Mesh& operator=(const Mesh& m);

        /**
         * @brief Gets a pointer to positions vector.
         *
         * @return float *
        */
        float* getPositionsPointer();
        /**
         * @brief Gets a pointer to normals vector.
         *
         * @return float *
        */
        float* getNormalsPointer();
        /**
         * @brief Gets a pointer to text coordinates vector.
         *
         * @return float *
        */
        float* getTexCoordsPointer();
        /**
         * @brief Gets pointer to indices vector.
         *
         * @return unsigned int *
        */
        unsigned int* getIndicesPointer();
        void updateVecs();

    private:

        /**
         * @brief Calculates bounds for a mesh.
         *
         * @param center[]
         * @param width
         * @param height
         * @param length
         * @param radius
         * @param extent[]
        */
        void bounds(float center[3], float &width, float &height,
        float &length, float &radius, float extent[3]) const;


        std::vector<assembly3d::Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<assembly3d::Group> m_groups;

        std::vector<float> m_positions;
        std::vector<float> m_normals;
        std::vector<float> m_texCoords;

        MeshFormat m_format;

        std::string m_meshPath;

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
        os << "---------------------------";
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

    inline assembly3d::Vertex& Mesh::getVertex(unsigned int index)
    { return m_vertices[index]; }

    inline assembly3d::Group& Mesh::getGroup(unsigned int index)
    { return m_groups[index]; }

    inline unsigned int* Mesh::getTriangle(unsigned int index)
    { return &m_indices[index * 3]; }

    inline int Mesh::getNumberOfTriangles() const
    { return m_numTriangles; }

    inline unsigned int Mesh::getNumberOfVertices() const
    { return static_cast<unsigned int>(m_vertices.size()); }

    inline unsigned int Mesh::getNumberOfGroups() const
    { return static_cast<unsigned int>(m_groups.size()); }

    inline bool Mesh::hasPositions() const
    { return m_hasPositions; }

    inline bool Mesh::hasNormals() const
    { return m_hasNormals; }

    inline bool Mesh::hasTexCoords() const
    { return m_hasTexCoords; }

    inline bool Mesh::hasTangents() const
    { return m_hasTangents; }

    inline bool Mesh::hasBitangents() const
    { return m_hasBitangents; }

    inline Mesh::MeshFormat& Mesh::getMeshFormat()
    { return m_format; }

    inline const char* Mesh::getMeshPath() const
    { return m_meshPath.c_str(); }

    inline float* Mesh::getPositionsPointer()
    { return &m_positions[0]; }

    inline float* Mesh::getNormalsPointer()
    { return &m_normals[0]; }

    inline float* Mesh::getTexCoordsPointer()
    { return &m_texCoords[0]; }

    inline unsigned int* Mesh::getIndicesPointer()
    { return &m_indices[0]; }

}
#endif  // _MESH_H_
