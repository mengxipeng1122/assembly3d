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

#include "PartialDisk.h"

#include <iostream>
#include <cmath>

using namespace std;
using namespace assembly3d;
using namespace assembly3d::prim::mesh;

#define PIf		3.1415926535897932384626433832795f

PartialDisk::PartialDisk(float inner, float outer, int slices,
                         int stacks, float start, float sweep)
    :
    Primitive(slices, stacks),
    m_inner(inner),
    m_outer(outer),
    m_start(start),
    m_sweep(sweep)
{
}

PartialDisk::~PartialDisk()
{

}

void PartialDisk::create(Mesh* mesh, bool positions, bool normals,
                         bool texCoords, bool tangents, bool bitangents)
{
    cout << "Create cylinder" << endl;

    for(int stack = 0; stack <= m_stacks; ++stack)
    {
        float t = (float)stack / (float)m_stacks;
        float r = m_inner * (1-t) + m_outer * t;

        for(int slice = 0; slice <= m_slices; ++slice)
        {
            float cosinus,sinus;
            {
                float angle = -2.0*PIf*slice/m_slices;
                cosinus = cos(angle);
                sinus = sin(angle);
            }

            float q = r / m_outer;

            Vertex vert = {{0.0f,0.0f,0.0f},
                           {0.0f,0.0f,0.0f},
                           {0.0f,0.0f},
                           {0.0f,0.0f,0.0f},
                           {0.0f,0.0f,0.0f}};

            vert.position[0] = r * cosinus;
            vert.position[1] = r * sinus;
            vert.position[2] = 0.0f;

            vert.normal[0] = 0.0f;
            vert.normal[1] = 0.0f;
            vert.normal[2] = 1.0f;

            vert.texCoord[0] = 0.5f * (q*cosinus+1);
            vert.texCoord[1] = 0.5f * (q*sinus+1);

            mesh->addVertex(vert);
        }
    }

    mesh->hasPositions(positions);
    mesh->hasNormals(normals);
    mesh->hasTexCoords(texCoords);
    mesh->hasTangents(tangents);
    mesh->hasBitangents(bitangents);
    mesh->initializeMeshFormat();

    calculateIndices(mesh);

    int numberTriangles = m_slices * m_stacks * 2;
    mesh->setNumTriangles(numberTriangles);
    Group g = {"PartialDisk", 0, numberTriangles};
    mesh->addGroup(g);


}
