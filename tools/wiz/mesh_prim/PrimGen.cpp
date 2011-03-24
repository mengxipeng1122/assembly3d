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

#include "PrimGen.h"
#include "primitives/Plane.h"
#include "primitives/Cube.h"
#include "primitives/Sphere.h"
#include "primitives/Torus.h"
#include "primitives/Trapezoid.h"
#include "primitives/Cylinder.h"
#include "primitives/Disk.h"
#include "primitives/PartialCylinder.h"
#include "primitives/PartialDisk.h"
#include "primitives/Rectangle.h"

using namespace std;
using namespace assembly3d;
using namespace assembly3d::prim::mesh;

PrimGen::PrimGen(bool generatePositions, bool generateNormals, bool generateTexCoords,
                 bool generateTangents, bool generateBitangents)
                     :
                     m_prim(0),
                     m_generatePositions(generatePositions),
                     m_generateNormals(generateNormals),
                     m_generateTexCoords(generateTexCoords),
                     m_generateTangents(generateTangents),
                     m_generateBitangents(generateBitangents)
{
}

PrimGen::~PrimGen()
{
    SAFE_DELETE(m_prim)
}

void PrimGen::createMesh(Mesh* mesh, int primType, std::vector<float> values)
{
    switch(primType)
    {
    case PRIM_TYPE_PLANE:
        m_prim = new Plane(values[0]);
        break;
    case PRIM_TYPE_CUBE:
        m_prim = new Cube(values[0]);
        break;
    case PRIM_TYPE_SPHERE:
        m_prim = new Sphere(values[0], (int)values[1], (int)values[2]);
        break;
    case PRIM_TYPE_TORUS:
        m_prim = new Torus(values[0], values[1], (int)values[2], (int)values[3]);
        break;
    case PRIM_TYPE_TRAPEZOID:
        m_prim = new Trapezoid(values[0], values[1], values[2]);
        break;
    case PRIM_TYPE_CYLINDER:
        m_prim = new Cylinder(values[0], values[1], values[2],
                              (int)values[3], (int)values[4]);
        break;
    case PRIM_TYPE_DISK:
        m_prim = new Disk(values[0], values[1], (int)values[2], (int)values[3]);
        break;
    case PRIM_TYPE_PARTIAL_CYLINDER:
        m_prim = new PartialCylinder(values[0], values[1], values[2], (int)values[3],
                                     (int)values[4], values[5], values[6]);
        break;
    case PRIM_TYPE_PARTIAL_DISK:
        m_prim = new PartialDisk(values[0], values[1], (int)values[2], (int)values[3],
                                 values[4], values[5]);
        break;
    case PRIM_TYPE_RECTANGLE:
        m_prim = new Rectangle(values[0], values[1], (int)values[2], (int)values[3]);
        break;
    default:
        m_prim = 0;
    }
    m_prim->create(mesh, m_generatePositions, m_generateNormals, m_generateTexCoords,
                   m_generateTangents, m_generateBitangents);

}
