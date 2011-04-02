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
#include "TransformTool.h"
#include <cmath>

#define PIf		3.1415926535897932384626433832795f

using namespace assembly3d;
using namespace assembly3d::wiz;

TransformTool::TransformTool()
{
    
}

TransformTool::~TransformTool()
{
    
}

void TransformTool::translate(Mesh* m, float tx, float ty, float tz)
{
    float tMatrix[3][4] = {{1.0f, 0.0f, 0.0f, tx},
                           {0.0f, 1.0f, 0.0f, ty},
                           {0.0f, 0.0f, 1.0f, tz}};
    
    float tiMatrix[3][3] = {{tMatrix[0][0], tMatrix[0][1], tMatrix[0][2]},
                            {tMatrix[1][0], tMatrix[1][1], tMatrix[1][2]},
                            {tMatrix[2][0], tMatrix[2][1], tMatrix[2][2]}};
    
//    for(int i=0; i<4; i++)
//    {
//        std::cout << tMatrix[i][0] << tMatrix[i][1] << tMatrix[i][2] << tMatrix[i][3] << std::endl;
//    }
                       
	for(unsigned int i = 0; i < m->getNumberOfVertices(); ++i)
	{
		Vertex* pVertex = &m->getVertex(i);

        multiplyVertexWithTransformMatrix(pVertex, tMatrix, tiMatrix);
    }
}

void TransformTool::rotate(Mesh* m, float rangle, float rx, float ry, float rz)
{
    // normalize rotation axis
    float rvec[3] = {rx, ry, rz};
    normalize(rvec);
    rx = rvec[0];
    ry = rvec[1];
    rz = rvec[2];

    float s = sin(2.0f*PIf*rangle/360.0f);
    float c = cos(2.0f*PIf*rangle/360.0f);
    float t = 1.0f - c;

    float rMatrix[3][4] = {{rx*rx*t + c,      rx*ry*t - rz*s,   rx*rz*t + ry*s,   0.0f},
                           {ry*rx*t + rz*s,   ry*ry*t + c,      ry*rz*t - rx*s,   0.0f},
                           {rz*rx*t - ry*s,   rz*ry*t + rx*s,   rz*rz*t + c,      0.0f}};
                           
    float riMatrix[3][3] = {{rMatrix[0][0], rMatrix[0][1], rMatrix[0][2]},
                            {rMatrix[1][0], rMatrix[1][1], rMatrix[1][2]},
                            {rMatrix[2][0], rMatrix[2][1], rMatrix[2][2]}};
    
                           
	for(unsigned int i = 0; i < m->getNumberOfVertices(); ++i)
	{
		Vertex* pVertex = &m->getVertex(i);

        multiplyVertexWithTransformMatrix(pVertex, rMatrix, riMatrix);
    }
    
}

void TransformTool::scale(Mesh* m, float sx, float sy, float sz)
{
    float sMatrix[3][4] = {{sx, 0.0f, 0.0f, 0.0f},
                           {0.0f, sy, 0.0f, 0.0f},
                           {0.0f, 0.0f, sz, 0.0f}};
    
    float siMatrix[3][3] = {{1.0f/sx, 0.0f, 0.0f},
                           {0.0f, 1.0f/sy, 0.0f},
                           {0.0f, 0.0f, 1.0f/sz}};
                           
	for(unsigned int i = 0; i < m->getNumberOfVertices(); ++i)
	{
		Vertex* pVertex = &m->getVertex(i);
        
        multiplyVertexWithTransformMatrix(pVertex, sMatrix, siMatrix);
        
    }
}

void TransformTool::resize(Mesh* m, float width, float height, float length)
{
    float rsx = width/m->getWidth();
    float rsy = height/m->getHeight();
    float rsz = length/m->getLength();
    
    scale(m, rsx, rsy, rsz);
}

void TransformTool::resize(Mesh* m, const char axis, float val)
{
    float rsx, rsy, rsz;
    rsx = rsy = rsz = 0.0f;
    if(axis == 'x')
    {
        rsx = val/m->getWidth();
        rsy = rsx;
        rsz = rsx;
    }
    else if(axis == 'y')
    {
        rsy = val/m->getHeight();
        rsx = rsy;
        rsz = rsy;
    }
    else if(axis == 'z')
    {
        rsz = val/m->getLength();
        rsx = rsz;
        rsy = rsz;
    }
    
    scale(m, rsx, rsy, rsz);
}

void TransformTool::center(Mesh* m, int ax, int ay, int az)
{
    float tX, tY, tZ;
    tX = tY = tZ = 0.0f;
    float centerX, centerY, centerZ;
    centerX = centerY = centerZ = 0.0f;
    m->getCenter(centerX, centerY, centerZ);
    if(ax)
    {
        tX = -centerX;
    }
    if(ay)
    {
        tY = -centerY;
    }
    if(az)
    {
        tZ = -centerZ;
    }
    
    translate(m, tX, tY, tZ);
}

void TransformTool::multiplyVertexWithTransformMatrix(Vertex* vec, float matrix[3][4], float inverseTransposedMatrix[3][3])
{
    float x,y,z;
    x = vec->position[0]; y = vec->position[1]; z = vec->position[2];
    vec->position[0] = x*matrix[0][0] + y*matrix[0][1] + z*matrix[0][2] + matrix[0][3];
    vec->position[1] = x*matrix[1][0] + y*matrix[1][1] + z*matrix[1][2] + matrix[1][3];
    vec->position[2] = x*matrix[2][0] + y*matrix[2][1] + z*matrix[2][2] + matrix[2][3];

    //normale, tangenten, bitangen inverse transpose
    x = vec->normal[0]; y = vec->normal[1]; z = vec->normal[2];
    vec->normal[0] = x*inverseTransposedMatrix[0][0] + y*inverseTransposedMatrix[0][1] + z*inverseTransposedMatrix[0][2];
    vec->normal[1] = x*inverseTransposedMatrix[1][0] + y*inverseTransposedMatrix[1][1] + z*inverseTransposedMatrix[1][2];
    vec->normal[2] = x*inverseTransposedMatrix[2][0] + y*inverseTransposedMatrix[2][1] + z*inverseTransposedMatrix[2][2];
    normalize(vec->normal);

    x = vec->bitangent[0]; y = vec->bitangent[1]; z = vec->bitangent[2];
    vec->bitangent[0] = x*inverseTransposedMatrix[0][0] + y*inverseTransposedMatrix[0][1] + z*inverseTransposedMatrix[0][2];
    vec->bitangent[1] = x*inverseTransposedMatrix[1][0] + y*inverseTransposedMatrix[1][1] + z*inverseTransposedMatrix[1][2];
    vec->bitangent[2] = x*inverseTransposedMatrix[2][0] + y*inverseTransposedMatrix[2][1] + z*inverseTransposedMatrix[2][2];
    normalize(vec->bitangent);

    x = vec->tangent[0]; y = vec->tangent[1]; z = vec->tangent[2];
    vec->tangent[0] = x*inverseTransposedMatrix[0][0] + y*inverseTransposedMatrix[0][1] + z*inverseTransposedMatrix[0][2];
    vec->tangent[1] = x*inverseTransposedMatrix[1][0] + y*inverseTransposedMatrix[1][1] + z*inverseTransposedMatrix[1][2];
    vec->tangent[2] = x*inverseTransposedMatrix[2][0] + y*inverseTransposedMatrix[2][1] + z*inverseTransposedMatrix[2][2];
    normalize(vec->tangent);

}

void TransformTool::normalize(float vector[3])
{
    float length = 0.0f;
    length = sqrtf(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);

    if(length == 0.0f)
        length = 0.0f;
    else
        length = 1.0f / length;

    vector[0] *= length;
    vector[1] *= length;
    vector[2] *= length;
}
