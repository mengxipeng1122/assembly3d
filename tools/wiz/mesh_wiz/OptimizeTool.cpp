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

#include "MeshWizIncludes.h"
#include "OptimizeTool.h"

using namespace assembly3d;
using namespace assembly3d::wiz;

OptimizeTool::OptimizeTool()
{
    
}

OptimizeTool::~OptimizeTool()
{
    
}
bool OptimizeTool::isInRange(float lhs, float rhs, float eps)
{
    if(lhs > rhs + eps || lhs < rhs - eps)
        return false;
    else
        return true;
}

bool OptimizeTool::vertexPosEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.position[0], rhs.position[0], eps) &&
       isInRange(lhs.position[1], rhs.position[1], eps) &&
       isInRange(lhs.position[2], rhs.position[2], eps) &&
	   isInRange(lhs.position[3], rhs.position[3], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool OptimizeTool::vertexNormalEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.normal[0], rhs.normal[0], eps) &&
       isInRange(lhs.normal[1], rhs.normal[1], eps) &&
       isInRange(lhs.normal[2], rhs.normal[2], eps) &&
	   isInRange(lhs.normal[3], rhs.normal[3], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool OptimizeTool::vertexTexCoordEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.texCoord[0], rhs.texCoord[0], eps) &&
       isInRange(lhs.texCoord[1], rhs.texCoord[1], eps) &&
	   isInRange(lhs.texCoord[2], rhs.texCoord[2], eps) &&
       isInRange(lhs.texCoord[3], rhs.texCoord[3], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool OptimizeTool::vertexTangentEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.tangent[0], rhs.tangent[0], eps) &&
       isInRange(lhs.tangent[1], rhs.tangent[1], eps) &&
       isInRange(lhs.tangent[2], rhs.tangent[2], eps) &&
	   isInRange(lhs.tangent[3], rhs.tangent[3], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool OptimizeTool::vertexBitangentEquals(const Vertex& lhs, const Vertex& rhs, float eps)
{
    if(isInRange(lhs.bitangent[0], rhs.bitangent[0], eps) &&
       isInRange(lhs.bitangent[1], rhs.bitangent[1], eps) &&
       isInRange(lhs.bitangent[2], rhs.bitangent[2], eps) &&
	   isInRange(lhs.bitangent[3], rhs.bitangent[3], eps))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int OptimizeTool::findFirstIndexEqualsEps(std::vector<Vertex> verts, Vertex v, Mesh::AttributeType a, float e)
{
    float epsilon[5] = {0.0f};
    switch(a)
    {
    case Mesh::POSITION:
        epsilon[0] = e;
        break;
    case Mesh::NORMAL:
        epsilon[1] = e;
        break;
    case Mesh::TEXCOORD:
        epsilon[2] = e;
        break;
    case Mesh::TANGENT:
        epsilon[3] = e;
        break;
    case Mesh::BITANGENT:
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

bool OptimizeTool::vertexEquals(const Vertex& lhs, const Vertex& rhs)
{
    bool positionEqual = false;
    bool normalEqual = false;
    bool texureEqual = false;
    bool tangentEqual = false;
    bool bitangentEqual = false;

    if(lhs.position[0] == rhs.position[0] &&
       lhs.position[1] == rhs.position[1] &&
       lhs.position[2] == rhs.position[2] &&
	   lhs.position[3] == rhs.position[3])
    {
        positionEqual = true;
    }
    else
    {
        positionEqual = false;
    }
    if(lhs.normal[0] == rhs.normal[0] &&
       lhs.normal[1] == rhs.normal[1] &&
       lhs.normal[2] == rhs.normal[2] &&
	   lhs.normal[3] == rhs.normal[3])
    {
        normalEqual = true;
    }
    else
    {
        normalEqual = false;
    }
    if(lhs.texCoord[0] == rhs.texCoord[0] &&
       lhs.texCoord[1] == rhs.texCoord[1] &&
	   lhs.texCoord[2] == rhs.texCoord[2] &&
       lhs.texCoord[3] == rhs.texCoord[3])
    {
        texureEqual = true;
    }
    else
    {
        texureEqual = false;
    }
    if(lhs.tangent[0] == rhs.tangent[0] &&
       lhs.tangent[1] == rhs.tangent[1] &&
       lhs.tangent[2] == rhs.tangent[2] &&
	   lhs.tangent[3] == rhs.tangent[3])
    {
        tangentEqual = true;
    }
    else
    {
        tangentEqual = false;
    }
    if(lhs.bitangent[0] == rhs.bitangent[0] &&
       lhs.bitangent[1] == rhs.bitangent[1] &&
       lhs.bitangent[2] == rhs.bitangent[2] &&
	   lhs.bitangent[3] == rhs.bitangent[3])
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

int OptimizeTool::findFirstIndexEquals(std::vector<Vertex> verts, Vertex v)
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
    std::vector<Vertex> origVertices;
    getVertices(m, origVertices);
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
            Vertex v = origVertices[pTriangle[j]];

            int index = findFirstIndexEquals(newVertices, v);
            if(index < 0)
            {
                index = static_cast<int>(newVertices.size());
                newVertices.push_back(v);
            }
            newIndices[i*3+j] = index;
        }
    }

    m->clearVertices();
    for(std::vector<Vertex>::iterator it = newVertices.begin(); it != newVertices.end(); ++it)
    {
        m->addPosition(it->position);
		if(m->hasNormals())
		{
			m->addNormal(it->normal);
		}
		if(m->hasTexCoords())
		{
			m->addTexCoord(it->texCoord);
		}
		if(m->hasTangents())
		{
			m->addTangent(it->tangent);
		}
		if(m->hasBitangents())
		{
			m->addBitangent(it->bitangent);
		}

    }
    m->clearIndices();
    for(std::vector<unsigned int>::iterator it = newIndices.begin(); it != newIndices.end(); ++it)
    {
        m->addIndex(*it);
    }

}

void OptimizeTool::stitch(Mesh *m, Mesh::AttributeType a, float epsilon)
{
    std::vector<Vertex> origVertices;
    getVertices(m, origVertices);

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
            Vertex v = origVertices[pTriangle[j]];

            int index = findFirstIndexEqualsEps(newVertices, v, a, epsilon);
            if(index < 0)
            {
                index = static_cast<int>(newVertices.size());
                newVertices.push_back(v);
            }
            newIndices[i*3+j] = index;
        }
    }

    m->clearVertices();
    for(std::vector<Vertex>::iterator it = newVertices.begin(); it != newVertices.end(); ++it)
    {
        m->addPosition(it->position);
		if(m->hasNormals())
		{
			m->addNormal(it->normal);
		}
		if(m->hasTexCoords())
		{
			m->addTexCoord(it->texCoord);
		}
		if(m->hasTangents())
		{
			m->addTangent(it->tangent);
		}
		if(m->hasBitangents())
		{
			m->addBitangent(it->bitangent);
		}
    }
    m->clearIndices();
    for(std::vector<unsigned int>::iterator it = newIndices.begin(); it != newIndices.end(); ++it)
    {
        m->addIndex(*it);
    }

}

void OptimizeTool::getVertices(assembly3d::Mesh *m,
                               std::vector<OptimizeTool::Vertex>& vertices)
{
    vertices.clear();
    for(int i = 0; i < m->getNumberOfVertices(); ++i)
    {
        Vertex vert = {{0.0f,0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f,0.0f},
                       {0.0f,0.0f,0.0f,0.0f}};

        vert.position[0] = m->getPosition(i)[0];
        vert.position[1] = m->getPosition(i)[1];
        vert.position[2] = m->getPosition(i)[2];
		vert.position[3] = m->getPosition(i)[3];

		if(m->hasNormals())
		{
			vert.normal[0] = m->getNormal(i)[0];
			vert.normal[1] = m->getNormal(i)[1];
			vert.normal[2] = m->getNormal(i)[2];
			vert.normal[3] = m->getNormal(i)[3];
		}

		if(m->hasTexCoords())
		{
			vert.texCoord[0] = m->getTexCoord(i)[0];
			vert.texCoord[1] = m->getTexCoord(i)[1];
			vert.texCoord[2] = m->getTexCoord(i)[2];
			vert.texCoord[3] = m->getTexCoord(i)[3];
		}
		if(m->hasTangents())
		{
			vert.tangent[0] = m->getTangent(i)[0];
			vert.tangent[1] = m->getTangent(i)[1];
			vert.tangent[2] = m->getTangent(i)[2];
			vert.tangent[3] = m->getTangent(i)[3];
		}

		if(m->hasBitangents())
		{
			vert.bitangent[0] = m->getBitangent(i)[0];
			vert.bitangent[1] = m->getBitangent(i)[1];
			vert.bitangent[2] = m->getBitangent(i)[2];
			vert.bitangent[3] = m->getBitangent(i)[3];
		}
        vertices.push_back(vert);
    }
}

