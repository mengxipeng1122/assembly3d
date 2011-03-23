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

#include "Cube.h"

#include <iostream>
using namespace std;
using namespace assembly3d;
using namespace assembly3d::prim::mesh;

Cube::Cube(float halfExtend)
    :
    m_halfExtend(halfExtend)
{
}

Cube::~Cube()
{
}

void Cube::create(Mesh* mesh, bool positions, bool normals,
                  bool texCoords, bool tangents, bool bitangents)
{
    cout << "Create Cube" << endl;

    int numVertices = 24;
    int numTriangles = 12;

    float cube_positions[] =
    {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, -1.0f
    };

    float cube_normals[] =
    {
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, +1.0f,  0.0f,
        0.0f, +1.0f,  0.0f,
        0.0f, +1.0f,  0.0f,
        0.0f, +1.0f,  0.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, +1.0f,
        0.0f,  0.0f, +1.0f,
        0.0f,  0.0f, +1.0f,
        0.0f,  0.0f, +1.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        +1.0f,  0.0f,  0.0f,
        +1.0f,  0.0f,  0.0f,
        +1.0f,  0.0f,  0.0f,
        +1.0f,  0.0f,  0.0f
    };


    float cube_texCoords[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };

    unsigned int cube_indices[] =
    {
        0, 2, 1,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 15, 14,
        12, 14, 13,
        16, 17, 18,
        16, 18, 19,
        20, 23, 22,
        20, 22, 21
    };


    for (int i = 0; i < numVertices; i++)
    {
        Vertex vert = {{0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f},
                       {0.0f,0.0f},
                       {0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f}};

        vert.position[0] = cube_positions[i*3 + 0];
        vert.position[1] = cube_positions[i*3 + 1];
        vert.position[2] = cube_positions[i*3 + 2];

        vert.normal[0] = cube_normals[i*3 + 0];
        vert.normal[1] = cube_normals[i*3 + 1];
        vert.normal[2] = cube_normals[i*3 + 2];

        vert.texCoord[0] = cube_texCoords[i*3 + 0];
        vert.texCoord[1] = cube_texCoords[i*3 + 1];

        vert.position[0] *= m_halfExtend;
        vert.position[1] *= m_halfExtend;
        vert.position[2] *= m_halfExtend;

        mesh->addVertex(vert);
    }

    mesh->hasPositions(positions);
    mesh->hasNormals(normals);
    mesh->hasTexCoords(texCoords);
    mesh->hasTangents(tangents);
    mesh->hasBitangents(bitangents);
    mesh->initializeMeshFormat();

    for(int i = 0; i < numTriangles*3; ++i)
    {
        mesh->addIndex(cube_indices[i]);
    }
    mesh->setNumTriangles(numTriangles);
    Group g = {"Cube", 0, numTriangles};
    mesh->addGroup(g);

}
