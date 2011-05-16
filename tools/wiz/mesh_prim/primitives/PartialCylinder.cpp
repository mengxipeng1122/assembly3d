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

#include "PartialCylinder.h"

#include <iostream>
#include <cmath>

using namespace std;
using namespace assembly3d;
using namespace assembly3d::prim;

#define PIf		3.1415926535897932384626433832795f

PartialCylinder::PartialCylinder(float base, float top, float height, int slices,
                                 int stacks, float start, float sweep)
    :
    Primitive(slices, stacks),
    m_base(base),
    m_top(top),
    m_height(height),
    m_start(start),
    m_sweep(sweep)
{
}

PartialCylinder::~PartialCylinder()
{

}

void PartialCylinder::create(Mesh* mesh, bool positions, bool normals,
                             bool texCoords, bool tangents, bool bitangents)
{
    cout << "Create PartialCylinder" << endl;

    std::vector<float> positionsVec;
    std::vector<float> normalsVec;
    std::vector<float> texCoordsVec;
    std::vector<float> tangentsVec;
    std::vector<float> bitangentsVec;

    for(int stack = 0; stack <= m_stacks; ++stack)
    {

        float nx, nz;
        {
            // x = top-base, y = height
            // (right) perpendicular
            // x' = y, y' = -x
            float x = m_height;
            float y = m_base - m_top;

            // unit length
            {
                float norm = sqrt(x*x + y*y);
                x /= norm;
                y /= norm;
            }

            //3D
            nx = x;
            nz = y;
        }
        for(int slice = 0; slice <= m_slices; ++slice)
        {
            float t = (float) stack / (float) m_stacks;
            float z = m_height*t;
            float r = m_base * (1-t) + m_top * t;

            float s = (float)slice / (float)m_slices;
            float cosinus, sinus;
            {
                float angle = m_start + m_sweep - (m_sweep*(float)slice / (float)m_slices);;
                cosinus = cos(angle);
                sinus = sin(angle);
            }

//            Vertex vert = {{0.0f,0.0f,0.0f},
//                           {0.0f,0.0f,0.0f},
//                           {0.0f,0.0f},
//                           {0.0f,0.0f,0.0f},
//                           {0.0f,0.0f,0.0f}};

//            vert.position[0] = r * sinus;
//            vert.position[1] = r * cosinus;
//            vert.position[2] = z;

//            vert.normal[0] = sinus * nx;
//            vert.normal[1] = cosinus * nx;
//            vert.normal[2] = nz;

//            vert.tangent[0] = 1.0f;
//            vert.tangent[1] = 0.0f;
//            vert.tangent[2] = 0.0f;

//            vert.texCoord[0] = s;
//            vert.texCoord[1] = t;

//            mesh->addVertex(vert);

            positionsVec.push_back(r * sinus);
            positionsVec.push_back(r * cosinus);
            positionsVec.push_back(z);

            normalsVec.push_back(sinus * nx);
            normalsVec.push_back(cosinus * nx);
            normalsVec.push_back(nz);

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

    Mesh::Group g = {(char*)"PartialCylinder", 0, numberOfTriangles()};
    mesh->addGroup(g);

}
