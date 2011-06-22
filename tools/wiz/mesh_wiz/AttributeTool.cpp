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
 
//------------------------------------------------------------------------------
// Code for generating normals and tangent taken from the obj loader class of
// Penjin game engine. http://code.google.com/p/penjin/
//------------------------------------------------------------------------------

#include "MeshWizIncludes.h"
#include "AttributeTool.h"
#include <cmath>

using namespace assembly3d;
using namespace assembly3d::wiz;

AttributeTool::AttributeTool()
{
    
}

AttributeTool::~AttributeTool()
{
    
}

void AttributeTool::removeNormals(Mesh* m)
{
    for(int i = 0; i < m->getNumberOfVertices(); ++i)
    {
        m->getNormal(i)[0] = 0.0f;
        m->getNormal(i)[1] = 0.0f;
        m->getNormal(i)[2] = 0.0f;
    }
    m->removeAttribute("NORMAL");
    m->hasNormals(false);
}

void AttributeTool::removeTangents(Mesh* m)
{
    for(int i = 0; i < m->getNumberOfVertices(); ++i)
    {
        m->getTangent(i)[0] = 0.0f;
        m->getTangent(i)[1] = 0.0f;
        m->getTangent(i)[2] = 0.0f;
    }
    m->removeAttribute("TANGENT");
    m->hasTangents(false);
}

void AttributeTool::removeBitangents(Mesh* m)
{
        for(int i = 0; i < m->getNumberOfVertices(); ++i)
    {
        m->getBitangent(i)[0] = 0.0f;
        m->getBitangent(i)[1] = 0.0f;
        m->getBitangent(i)[2] = 0.0f;
    }
    m->removeAttribute("BITANGENT");
    m->hasBitangents(false);
}


void AttributeTool::addNormals(Mesh* m)
{
    m->generateNormals();
    m->addAttribute("NORMAL", 3, "FLOAT");
}

void AttributeTool::addTangents(Mesh* m)
{
    if(m->hasTangents() == false)
    {
        m->generateTangents();
    }
    m->addAttribute("TANGENT", 3, "FLOAT");
}

void AttributeTool::addBitangents(Mesh* m)
{
    if(m->hasBitangents() == false)
    {
        m->generateTangents();
    }
    m->addAttribute("BITANGENT", 3, "FLOAT");
}

