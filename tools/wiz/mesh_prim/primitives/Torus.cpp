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

#include "Torus.h"

#include <cmath>
#include <iostream>
using namespace std;
using namespace assembly3d;
using namespace assembly3d::prim;

#define PIf		3.1415926535897932384626433832795f

Torus::Torus(float inner, float outer, int slices, int stacks)
    :
    Primitive(slices, stacks),
    m_inner(inner),
    m_outer(outer)
{
}

Torus::~Torus()
{
}

void Torus::create(Mesh* mesh, bool positions, bool normals,
                   bool texCoords, bool tangents, bool bitangents)
{
    cout << "Create torus" << endl;

    // t, s = parametric values of the equations, in the range [0,1]
    float t = 0;
    float s = 0;

    // incr_t, incr_s are increment values aplied to t and s on each loop iteration	to generate the torus
    float tIncr ;
    float sIncr ;

    // to store precomputed sin and cos values
    float cos2PIt, sin2PIt, cos2PIs, sin2PIs;

    tIncr = 1.0f/(float)m_slices;
    sIncr = 1.0f/(float)m_stacks;

    std::vector<float> positionsVec;
    std::vector<float> normalsVec;
    std::vector<float> texCoordsVec;
    std::vector<float> tangentsVec;
    std::vector<float> bitangentsVec;

    // generate vertices and its attributes
    for (int stack = 0; stack <= m_stacks; ++stack, s+=sIncr)
    {
        // precompute some values
        cos2PIs = (float)cos (2.0f*PIf*s);
        sin2PIs = (float)sin (2.0f*PIf*s);

        t=0.0f;
        for (int slice = 0; slice <= m_slices; ++slice, t+=tIncr)
        {
            // precompute some values
            cos2PIt = cos(2.0f*PIf*t);
            sin2PIt = sin(2.0f*PIf*t);

//            Vertex vert = {{0.0f,0.0f,0.0f},
//                           {0.0f,0.0f,0.0f},
//                           {0.0f,0.0f},
//                           {0.0f,0.0f,0.0f},
//                           {0.0f,0.0f,0.0f}};

//            vert.position[0] = (m_outer + m_inner * cos2PIt) * cos2PIs;
//            vert.position[1] = (m_outer + m_inner * cos2PIt) * sin2PIs;
//            vert.position[2] = m_inner * sin2PIt;

//            // generate normal and stores it in the right position
//            // NOTE: cos (2PIx) = cos (x) and sin (2PIx) = sin (x) so, we can use this formula
//            //       normal = {cos(2PIs)cos(2PIt) , sin(2PIs)cos(2PIt) ,sin(2PIt)}
//            vert.normal[0] = cos2PIs * cos2PIt;
//            vert.normal[1] = sin2PIs * cos2PIt;
//            vert.normal[2] = sin2PIt;

//            vert.texCoord[0] = t;
//            vert.texCoord[1] = s;

//            mesh->addVertex(vert);

            float x = (m_outer + m_inner * cos2PIt) * cos2PIs;
            float y = (m_outer + m_inner * cos2PIt) * sin2PIs;
            float z = m_inner * sin2PIt;

            positionsVec.push_back(x);
            positionsVec.push_back(y);
            positionsVec.push_back(z);

            // generate normal and stores it in the right position
            // NOTE: cos (2PIx) = cos (x) and sin (2PIx) = sin (x) so, we can use this formula
            //       normal = {cos(2PIs)cos(2PIt) , sin(2PIs)cos(2PIt) ,sin(2PIt)}
            normalsVec.push_back(cos2PIs * cos2PIt);
            normalsVec.push_back(sin2PIs * cos2PIt);
            normalsVec.push_back(sin2PIt);

            texCoordsVec.push_back(t);
            texCoordsVec.push_back(s);

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

    Mesh::Group g = {(char*)"Torus", 0, numberOfTriangles()};
    mesh->addGroup(g);

}

