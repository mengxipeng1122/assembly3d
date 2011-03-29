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

#include "FrontFaceTool.h"
#include <cmath>
#include <sstream>

#define PIf		3.1415926535897932384626433832795f

using namespace assembly3d;
using namespace assembly3d::wiz::mesh;

FrontFaceTool::FrontFaceTool()
{
}

FrontFaceTool::~FrontFaceTool()
{
}

void FrontFaceTool::flip(Mesh* mesh)
{
    int numTriangles = mesh->getNumberOfTriangles();
    for(int i = 0; i < numTriangles; ++i)
    {
        unsigned int* pTriangle = mesh->getTriangle(i);
        unsigned int idx1 = pTriangle[1];
        unsigned int idx2 = pTriangle[2];

        pTriangle[1] = idx2;
        pTriangle[2] = idx1;
    }

    int numVertices = mesh->getNumberOfVertices();
    for(int i = 0; i < numVertices; ++i)
    {
        Vertex* vert = &mesh->getVertex(i);

        vert->normal[0] *= -1.0f;
        vert->normal[1] *= -1.0f;
        vert->normal[2] *= -1.0f;
    }
}

bool FrontFaceTool::testNormalConsitency(Mesh *mesh, std::string& resultMsg)
{
    bool modelChanged = false;
    Mesh* mesh2 = new Mesh(*mesh);
    mesh2->generateNormals();

    std::vector<int> verticesOutwards;
    std::vector<int> verticesInwards;

    std::stringstream strStr;

    for(size_t i = 0; i < mesh->getNumberOfVertices(); ++i)
    {
        Vertex* v1 = &mesh->getVertex(i);
        Vertex* v2 = &mesh2->getVertex(i);

        float dotVec1Vec2 =  v1->normal[0]*v2->normal[0] +
                             v1->normal[1]*v2->normal[1] +
                             v1->normal[2]*v2->normal[2];

        float angle = (float)acos(dotVec1Vec2);

        if(angle < (PIf / 2))
        {
            verticesOutwards.push_back(i);
        }
        else
        {
            verticesInwards.push_back(i);
        }
    }
    SAFE_DELETE(mesh2)

    strStr << verticesOutwards.size() << " outward vertices.\n";
    strStr << verticesInwards.size() << " inward vertices.\n";

    if(verticesOutwards.size() > verticesInwards.size() && !verticesInwards.empty())
    {
        strStr << "Flipping inward vertices...\n";
        for(size_t i = 0; i < verticesInwards.size(); ++i)
        {
            Vertex* vert = &mesh->getVertex(verticesInwards[i]);

            vert->normal[0] *= -1.0f;
            vert->normal[1] *= -1.0f;
            vert->normal[2] *= -1.0f;
        }
        modelChanged = true;
    }
    else if(verticesOutwards.size() < verticesInwards.size() && !verticesOutwards.empty())
    {
        strStr << "Flipping outward vertices...\n";
        for(size_t i = 0; i < verticesOutwards.size(); ++i)
        {
            Vertex* vert = &mesh->getVertex(verticesOutwards[i]);

            vert->normal[0] *= -1.0f;
            vert->normal[1] *= -1.0f;
            vert->normal[2] *= -1.0f;
        }
        modelChanged = true;
    }
    resultMsg = strStr.str();
    return modelChanged;
}
