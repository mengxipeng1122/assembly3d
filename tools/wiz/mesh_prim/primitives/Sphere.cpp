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

#include "Sphere.h"

#include <cmath>
#include <iostream>
using namespace std;
using namespace assembly3d;
using namespace prim;
using namespace mesh;

#define PIf		3.1415926535897932384626433832795f

Sphere::Sphere(float radius, int slices)
    :
    m_radius(radius),
    m_slices(slices)
{
}

Sphere::~Sphere()
{
}

void Sphere::create(Mesh* mesh,
                    bool positions,
                    bool normals,
                    bool texCoords,
                    bool tangents,
                    bool bitangents)
{
    cout << "Create sphere" << endl;

    unsigned int i,j;

    int numberParallels = m_slices;
    int numberIndices = numberParallels * m_slices * 6;
    int numberTriangles = numberIndices / 3;

    float angleStep = (2.0f * PIf) / ((float) m_slices);

    for (i = 0; i < (unsigned int)numberParallels + 1; i++ ) {
        for (j = 0; j < (unsigned int)m_slices + 1; j++ ) {

            Vertex vert = {{0.0f,0.0f,0.0f},
                           {0.0f,0.0f,0.0f},
                           {0.0f,0.0f},
                           {0.0f,0.0f,0.0f},
                           {0.0f,0.0f,0.0f}};

            vert.position[0] = m_radius * sinf ( angleStep * (float)i ) * sinf ( angleStep * (float)j );
            vert.position[1] = m_radius * cosf ( angleStep * (float)i );
            vert.position[2] = m_radius * sinf ( angleStep * (float)i ) * cosf ( angleStep * (float)j );

            vert.normal[0] = vert.position[0] / m_radius;
            vert.normal[1] = vert.position[1] / m_radius;
            vert.normal[2] = vert.position[2] / m_radius;

            vert.texCoord[0] = (float) j / (float) m_slices;
            vert.texCoord[1] = ( 1.0f - (float) i ) / (float) (numberParallels - 1 );

            mesh->addVertex(vert);
        }
    }

    mesh->hasPositions(positions);
    mesh->hasNormals(normals);
    mesh->hasTexCoords(texCoords);
    mesh->hasTangents(tangents);
    mesh->hasBitangents(bitangents);
    mesh->initializeMeshFormat();

    for (i = 0; i < (unsigned int)numberParallels ; i++ ) {
        for (j = 0; j < (unsigned int)m_slices; j++ ) {

            unsigned int tris[6] = {0,0,0,0,0,0};

            tris[0] = i * ( m_slices + 1 ) + j;
            tris[1] = ( i + 1 ) * ( m_slices + 1 ) + j;
            tris[2] = ( i + 1 ) * ( m_slices + 1 ) + ( j + 1 );

            tris[3] = i * ( m_slices + 1 ) + j;
            tris[4] = ( i + 1 ) * ( m_slices + 1 ) + ( j + 1 );
            tris[5] = i * ( m_slices + 1 ) + ( j + 1 );

            for(unsigned int k = 0; k < 6; ++k)
            {
                mesh->addIndex(tris[k]);
            }
        }
    }
    mesh->setNumTriangles(numberTriangles);
    Group g = {"sphere", 0, numberTriangles};
    mesh->addGroup(g);

}


