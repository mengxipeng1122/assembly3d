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
using namespace Wiz;

Cube::Cube(float halfExtend)
    :
    m_halfExtend(halfExtend)
{
}

Cube::~Cube()
{
}

void Cube::create(Mesh *mesh)
{
    cout << "Create cube" << endl;

    int numVertices = 24;
    int numTriangles = 12;

    float positions[] =
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

    float normals[] =
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


    float texCoords[] =
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

    unsigned int indices[] =
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
                       {0.0f,0.0f},
                       {0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f}};

        createVertex(vert, &positions[i*3], &normals[i*3], &texCoords[i*3]);
        mesh->addVertex(vert);
    }

    mesh->initializeStandardMeshFormat();

    for(int i = 0; i < numTriangles*3; ++i)
    {
        mesh->addIndex(indices[i]);
    }
    mesh->setNumTriangles(numTriangles);
    Group g;
    g.name = "xy_plane";
    g.startIndex = 0;
    g.triangleCount = numTriangles;
    mesh->addGroup(g);

}

void Cube::createVertex(Vertex& vertex, float position[3], float normal[3], float texCoord[2])
{

    vertex.position[0] = position[0];
    vertex.position[1] = position[1];
    vertex.position[2] = position[2];

    vertex.normal[0] = normal[0];
    vertex.normal[1] = normal[1];
    vertex.normal[2] = normal[2];

    vertex.texCoord[0] = texCoord[0];
    vertex.texCoord[1] = texCoord[1];

    vertex.position[0] *= m_halfExtend;
    vertex.position[1] *= m_halfExtend;
    vertex.position[2] *= m_halfExtend;

}
