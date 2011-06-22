/*
 * Copyright (c) 2011 Peter Vasil, Michael Nischt
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

#include "Trapezoid.h"

#include <iostream>

using namespace std;
using namespace assembly3d;
using namespace assembly3d::prim;

Trapezoid::Trapezoid(float base, float top, float height,
                     int slices, int stacks)
    :
    Primitive(slices, stacks),
    m_base(base),
    m_top(top),
    m_height(height)
{
}

Trapezoid::~Trapezoid()
{
}

void Trapezoid::create(Mesh* mesh, bool positions, bool normals,
                       bool texCoords, bool tangents, bool bitangents)
{
    cout << "Create Trapezoid" << endl;

    std::vector<float> positionsVec;
    std::vector<float> normalsVec;
    std::vector<float> texCoordsVec;
    std::vector<float> tangentsVec;
    std::vector<float> bitangentsVec;

    for(int stack = 0; stack <= m_stacks; ++stack)
    {
        float t = (float)stack / (float)m_stacks;

        float y = t * m_height;

        for(int slice = 0; slice <= m_slices; ++slice)
        {
            float s = (float)slice / (float)m_slices;

            float x = (2*s - 1) * ((1-t)*m_base + t*m_top );

//            Vertex vert = {{0.0f,0.0f,0.0f},
//                           {0.0f,0.0f,0.0f},
//                           {0.0f,0.0f},
//                           {0.0f,0.0f,0.0f},
//                           {0.0f,0.0f,0.0f}};

//            vert.position[0] = x;
//            vert.position[1] = y;
//            vert.position[2] = 0.0f;

//            vert.normal[0] = 0.0f;
//            vert.normal[1] = 0.0f;
//            vert.normal[2] = 1.0f;

//            vert.tangent[0] = 1.0f;
//            vert.tangent[1] = 0.0f;
//            vert.tangent[2] = 0.0f;

//            vert.texCoord[0] = s;
//            vert.texCoord[1] = t;

//            mesh->addVertex(vert);

            positionsVec.push_back(x);
            positionsVec.push_back(y);
            positionsVec.push_back(0.0f);

            normalsVec.push_back(0.0f);
            normalsVec.push_back(0.0f);
            normalsVec.push_back(1.0f);

            texCoordsVec.push_back(s);
            texCoordsVec.push_back(t);

            tangentsVec.push_back(1.0f);
            tangentsVec.push_back(0.0f);
            tangentsVec.push_back(0.0f);

        }
    }

    mesh->setPositions(positionsVec);
    mesh->hasPositions(positions);

    mesh->setNormals(normalsVec);
    mesh->hasNormals(normals);

    mesh->setTexCoords(texCoordsVec);
    mesh->hasTexCoords(texCoords);

    mesh->setTangents(tangentsVec);
    mesh->hasTangents(tangents);

    mesh->setBitangents(bitangentsVec);
    mesh->hasBitangents(bitangents);

    mesh->initializeMeshFormat();

    generateIndices(mesh);

    Mesh::Group g = {(char*)"Trapezoid", 0, numberOfTriangles()};
    mesh->addGroup(g);

}
