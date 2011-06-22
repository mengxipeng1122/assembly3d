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

#include "MeshTool.h"
#include "Mesh.h"
#include <cstring>

using namespace assembly3d;
using namespace assembly3d::wiz;

MeshTool::MeshTool()
{
}

void MeshTool::merge(Mesh *first, Mesh *second)
{
    for(int i = 0; i < second->getNumberOfVertices(); ++i)
    {
        float* pos = second->getPosition(i);
        first->addPosition(pos);
        if(first->hasNormals() && second->hasNormals())
        {
            float* normal = second->getNormal(i);
            first->addNormal(normal);
        }
        if(first->hasTexCoords() && second->hasTexCoords())
        {
            float* texCoord = second->getTexCoord(i);
            first->addTexCoord(texCoord);
        }
        if(first->hasTangents() && second->hasTangents())
        {
            float* tangent = second->getTangent(i);
            first->addTangent(tangent);
        }
        if(first->hasBitangents() && second->hasBitangents())
        {
            float* bitangent = second->getBitangent(i);
            first->addTangent(bitangent);
        }
    }
    unsigned int numIndices= first->getNumberOfTriangles()*3;
    for(int i = 0; i < second->getNumberOfTriangles(); ++i)
    {
        unsigned int* triangle = second->getTriangle(i);
        triangle[0] += numIndices;
        triangle[1] += numIndices;
        triangle[2] += numIndices;
        first->addIndex(triangle[0]);
        first->addIndex(triangle[1]);
        first->addIndex(triangle[2]);
    }
    for(int i = 0; i < second->getNumberOfGroups(); ++i)
    {
        Mesh::Group g;
        std::string tmpGroupName = second->getGroup(i).name;
        g.name = new char[tmpGroupName.length()+1];
        g.name[tmpGroupName.length()] = 0;
        memcpy(g.name, tmpGroupName.c_str(), tmpGroupName.size());

        g.triangleCount = second->getGroup(i).triangleCount;

        g.startIndex = second->getGroup(i).startIndex + numIndices;

        first->addGroup(g);
    }
}
