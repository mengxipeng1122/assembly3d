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

#include "OptimizeTool.h"

OptimizeTool::OptimizeTool()
{
    
}

OptimizeTool::~OptimizeTool()
{
    
}
bool isInRange(float lhs, float rhs, float eps)
{
    if(lhs > rhs + eps/2.0f || lhs < rhs - eps/2.0f)
        return false;
    else
        return true;
}

bool vertexPosEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.position[0], rhs.position[0], eps) &&
       isInRange(lhs.position[1], rhs.position[1], eps) &&
       isInRange(lhs.position[2], rhs.position[2], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool vertexNormalEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.normal[0], rhs.normal[0], eps) &&
       isInRange(lhs.normal[1], rhs.normal[1], eps) &&
       isInRange(lhs.normal[2], rhs.normal[2], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool vertexTexCoordEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.texCoord[0], rhs.texCoord[0], eps) &&
       isInRange(lhs.texCoord[1], rhs.texCoord[1], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool vertexTangentEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.tangent[0], rhs.tangent[0], eps) &&
       isInRange(lhs.tangent[1], rhs.tangent[1], eps) &&
       isInRange(lhs.tangent[2], rhs.tangent[2], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool vertexBitangentEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.bitangent[0], rhs.bitangent[0], eps) &&
       isInRange(lhs.bitangent[1], rhs.bitangent[1], eps) &&
       isInRange(lhs.bitangent[2], rhs.bitangent[2], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int findFirstIndexEqualsEps(std::vector<Vertex> verts, Vertex v, OptimizeTool::Attribute a, float e)
{
    float epsilon[5] = {0.0f};
    switch(a)
    {
    case OptimizeTool::POSITION:
        epsilon[0] = e;
        break;
    case OptimizeTool::NORMAL:
        epsilon[1] = e;
        break;
    case OptimizeTool::TEXCOORD:
        epsilon[2] = e;
        break;
    case OptimizeTool::TANGENT:
        epsilon[3] = e;
        break;
    case OptimizeTool::BITANGENT:
        epsilon[4] = e;
        break;
    }

    for(unsigned int i = 0; i < verts.size(); ++i)
    {

        if(vertexPosEquals(verts[i], v, epsilon[0]) &&
           vertexNormalEquals(verts[i], v, epsilon[1]) &&
           vertexTexCoordEquals(verts[i], v, epsilon[2]) &&
           vertexTangentEquals(verts[i], v, epsilon[3]) &&
           vertexBitangentEquals(verts[i], v, epsilon[4]))
        {
            return i;
        }
    }
    return -1;

}

bool vertexEquals(const Vertex& lhs, const Vertex& rhs)
{
    bool positionEqual = false;
    bool normalEqual = false;
    bool texureEqual = false;
    bool tangentEqual = false;
    bool bitangentEqual = false;

    if(lhs.position[0] == rhs.position[0] &&
       lhs.position[1] == rhs.position[1] &&
       lhs.position[2] == rhs.position[2])
    {
        positionEqual = true;
    }
    else
    {
        positionEqual = false;
    }
    if(lhs.normal[0] == rhs.normal[0] &&
       lhs.normal[1] == rhs.normal[1] &&
       lhs.normal[2] == rhs.normal[2])
    {
        normalEqual = true;
    }
    else
    {
        normalEqual = false;
    }
    if(lhs.texCoord[0] == rhs.texCoord[0] &&
       lhs.texCoord[1] == rhs.texCoord[1])
    {
        texureEqual = true;
    }
    else
    {
        texureEqual = false;
    }
    if(lhs.tangent[0] == rhs.tangent[0] &&
       lhs.tangent[1] == rhs.tangent[1] &&
       lhs.tangent[2] == rhs.tangent[2])
    {
        tangentEqual = true;
    }
    else
    {
        tangentEqual = false;
    }
    if(lhs.bitangent[0] == rhs.bitangent[0] &&
       lhs.bitangent[1] == rhs.bitangent[1] &&
       lhs.bitangent[2] == rhs.bitangent[2])
    {
        bitangentEqual = true;
    }
    else
    {
        bitangentEqual = false;
    }

    if(positionEqual && normalEqual && texureEqual &&
       tangentEqual && bitangentEqual)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int findFirstIndexEquals(std::vector<Vertex> verts, Vertex v)
{

    for(unsigned int i = 0; i < verts.size(); ++i)
    {

        if(vertexEquals(verts[i], v))
        {
            return i;
        }
    }
    return -1;
}


void OptimizeTool::stitch(Mesh *m)
{
//    int numIndices = m->getNumberOfTriangles()*3;

    std::vector<Vertex> newVertices;
    std::vector<unsigned int> newIndices;

    newIndices.reserve((size_t)(m->getNumberOfTriangles()*3));
    for(int i = 0; i < m->getNumberOfTriangles(); ++i)
    {
        const unsigned int* pTriangle = m->getTriangle(i);
        newIndices.push_back(pTriangle[0]);
        newIndices.push_back(pTriangle[1]);
        newIndices.push_back(pTriangle[2]);
    }

    for(int i = 0; i < m->getNumberOfTriangles(); ++i)
    {
        const unsigned int* pTriangle = m->getTriangle(i);
        for(int j = 0; j < 3; ++j)
        {
//            unsigned int ind = pTriangle[j];
            Vertex v = m->getVertex(pTriangle[j]);

            int index = findFirstIndexEquals(newVertices, v);
            if(index < 0)
            {
                index = newVertices.size();
                newVertices.push_back(v);
            }
            newIndices[i*3+j] = index;
        }
    }

    m->clearVertices();
    for(std::vector<Vertex>::iterator it = newVertices.begin(); it != newVertices.end(); ++it)
    {
        m->addVertex(*it);
    }
    m->clearIndices();
    for(std::vector<unsigned int>::iterator it = newIndices.begin(); it != newIndices.end(); ++it)
    {
        m->addIndex(*it);
    }

}

void OptimizeTool::stitch(Mesh *m, Attribute a, float epsilon)
{
//    int numIndices = m->getNumberOfTriangles()*3;

    std::vector<Vertex> newVertices;
    std::vector<unsigned int> newIndices;

    newIndices.reserve((size_t)(m->getNumberOfTriangles()*3));
    for(int i = 0; i < m->getNumberOfTriangles(); ++i)
    {
        const unsigned int* pTriangle = m->getTriangle(i);
        newIndices.push_back(pTriangle[0]);
        newIndices.push_back(pTriangle[1]);
        newIndices.push_back(pTriangle[2]);
    }

    for(int i = 0; i < m->getNumberOfTriangles(); ++i)
    {
        const unsigned int* pTriangle = m->getTriangle(i);
        for(int j = 0; j < 3; ++j)
        {
//            unsigned int ind = pTriangle[j];
            Vertex v = m->getVertex(pTriangle[j]);

            int index = findFirstIndexEqualsEps(newVertices, v, a, epsilon);
            if(index < 0)
            {
                index = newVertices.size();
                newVertices.push_back(v);
            }
            newIndices[i*3+j] = index;
        }
    }

    m->clearVertices();
    for(std::vector<Vertex>::iterator it = newVertices.begin(); it != newVertices.end(); ++it)
    {
        m->addVertex(*it);
    }
    m->clearIndices();
    for(std::vector<unsigned int>::iterator it = newIndices.begin(); it != newIndices.end(); ++it)
    {
        m->addIndex(*it);
    }

}

