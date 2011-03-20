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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "MeshPrimIncludes.h"
#include "Mesh.h"

class Primitive
{
public:
    Primitive() {}
    virtual ~Primitive(){}

    virtual void create(Mesh* mesh) = 0;

protected:
    void initializeStandardMeshFormat(Mesh::MeshFormat& format, unsigned int numVertices);
};

inline void Primitive::initializeStandardMeshFormat(Mesh::MeshFormat &format, unsigned int numVertices)
{
    format.name = "Assembly3D.mesh";
    format.isBinary = true;

    if(numVertices < (1 << 8))
        format.indexType = "UNSIGNED_BYTE";
    else if(numVertices < (1 << 16))
        format.indexType = "UNSIGNED_SHORT";
    else
        format.indexType = "UNSIGNED_INT";

    format.attributeCount = 3;
    format.attributeName.push_back("POSITION");
    format.attributeName.push_back("NORMAL");
    format.attributeName.push_back("TEXTURE");
    format.attributeSize.push_back(3);
    format.attributeSize.push_back(3);
    format.attributeSize.push_back(2);
    format.attributeType.push_back("FLOAT");
    format.attributeType.push_back("FLOAT");
    format.attributeType.push_back("FLOAT");

}

#endif // PRIMITIVE_H
