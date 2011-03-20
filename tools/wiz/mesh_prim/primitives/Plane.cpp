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

#include "Plane.h"

#include <iostream>
using namespace std;
using namespace Wiz;

Plane::Plane(float halfExtend)
    :
    m_halfExtend(halfExtend)
{

}

Plane::~Plane()
{
}

void Plane::create(Mesh* mesh)
{
    cout << "Create plane" << endl;

    int numberVertices = 4;
    int numTriangles = 2;

    float positions[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };

    float normals[] =
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    float texCoords[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        1, 3, 2
    };

    for (int i = 0; i < numberVertices; i++)
    {
        Vertex vert = {{0.0f,0.0f,0.0f},
                       {0.0f,0.0f},
                       {0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f}};

        createVertex(vert, &positions[i*3], &normals[i*3], &texCoords[i*3]);
        mesh->addVertex(vert);
    }

    Mesh::MeshFormat& format = mesh->getMeshFormat();
    initializeStandardMeshFormat(format, mesh->getNumberOfVertices());


    for(int i = 0; i < numTriangles*3; ++i)
    {
        mesh->addIndex(indices[i]);
    }
    mesh->setNumTriangles(numTriangles);
    Group g;
    g.name = "xy_plane";
    g.startIndex = 0;
    g.triangleCount = 2;
    mesh->addGroup(g);
}

void Plane::createVertex(Vertex& vertex, float position[3], float normal[3], float texCoord[2])
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

}


