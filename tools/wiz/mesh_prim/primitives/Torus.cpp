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
using namespace prim;
using namespace mesh;

#define PIf		3.1415926535897932384626433832795f

Torus::Torus(float innerRadius,
             float outerRadius,
             int numSides,
             int numFaces)
                 :
                 m_innerRadius(innerRadius),
                 m_outerRadius(outerRadius),
                 m_numSides(numSides),
                 m_numFaces(numFaces)
{
}

Torus::~Torus()
{
}

void Torus::create(Mesh* mesh,
                   bool positions,
                   bool normals,
                   bool texCoords,
                   bool tangents,
                   bool bitangents)
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

    // loop counters
    unsigned int sideCount, faceCount;

    // used to generate the indices
    unsigned int v0, v1, v2, v3;

//    int numberVertices = (m_numFaces+1) * (m_numSides+1);
    int numberTriangles = m_numFaces * m_numSides * 2; // 2 triangles per face

    tIncr = 1.0f/(float)m_numFaces;
    sIncr = 1.0f/(float)m_numSides;

    // generate vertices and its attributes
    for (sideCount = 0; sideCount <= m_numSides; ++sideCount, s+=sIncr)
    {
        // precompute some values
        cos2PIs = (float)cos (2.0f*PIf*s);
        sin2PIs = (float)sin (2.0f*PIf*s);

        t=0.0f;
        for (faceCount = 0; faceCount <= m_numFaces; ++faceCount, t+=tIncr)
        {
            // precompute some values
            cos2PIt = (float)cos (2.0f*(float)M_PI*t);
            sin2PIt = (float)sin (2.0f*(float)M_PI*t);

            Vertex vert = {{0.0f,0.0f,0.0f},
                           {0.0f,0.0f,0.0f},
                           {0.0f,0.0f},
                           {0.0f,0.0f,0.0f},
                           {0.0f,0.0f,0.0f}};

            // generate vertex and stores it in the right position
//            indexVertices = ((sideCount * (m_numFaces +1)) + faceCount)* 4;
            vert.position[0] = (m_outerRadius + m_innerRadius * cos2PIt) * cos2PIs;
            vert.position[1] = (m_outerRadius + m_innerRadius * cos2PIt) * sin2PIs;
            vert.position[2] = m_innerRadius * sin2PIt;

            // generate normal and stores it in the right position
            // NOTE: cos (2PIx) = cos (x) and sin (2PIx) = sin (x) so, we can use this formula
            //       normal = {cos(2PIs)cos(2PIt) , sin(2PIs)cos(2PIt) ,sin(2PIt)}
            vert.normal[0] = cos2PIs * cos2PIt;
            vert.normal[1] = sin2PIs * cos2PIt;
            vert.normal[2] = sin2PIt;

            // generate texture coordinates and stores it in the right position
            vert.texCoord[0] = t;
            vert.texCoord[1] = s;

            mesh->addVertex(vert);

        }
    }

    mesh->hasPositions(positions);
    mesh->hasNormals(normals);
    mesh->hasTexCoords(texCoords);
    mesh->hasTangents(tangents);
    mesh->hasBitangents(bitangents);
    mesh->initializeMeshFormat();

    // generate indices
    for (sideCount = 0; sideCount < m_numSides; ++sideCount)
    {
        for (faceCount = 0; faceCount < m_numFaces; ++faceCount)
        {
            // get the number of the vertices for a face of the torus. They must be < numVertices
            v0 = ((sideCount * (m_numFaces +1)) + faceCount);
            v1 = (((sideCount+1) * (m_numFaces +1)) + faceCount);
            v2 = (((sideCount+1) * (m_numFaces +1)) + (faceCount+1));
            v3 = ((sideCount * (m_numFaces +1)) + (faceCount+1));

            // first triangle of the face, counter clock wise winding
            mesh->addIndex(v0);
            mesh->addIndex(v1);
            mesh->addIndex(v2);

            // second triangle of the face, counter clock wise winding
            mesh->addIndex(v0);
            mesh->addIndex(v2);
            mesh->addIndex(v3);
        }
    }
    mesh->setNumTriangles(numberTriangles);
    Group g = {"torus", 0, numberTriangles};
    mesh->addGroup(g);

}

