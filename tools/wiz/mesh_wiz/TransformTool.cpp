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

#include "TransformTool.h"
#include <cmath>

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
    
    for(int i=0; i<4; i++)
    {
        std::cout << tMatrix[i][0] << tMatrix[i][1] << tMatrix[i][2] << tMatrix[i][3] << std::endl;
    }
                       
	for(unsigned int i = 0; i < m->getNumberOfVertices(); ++i)
	{
		Vertex* pVertex = &m->getVertex(i);

		calculateVectorWithMatrix(pVertex, tMatrix, tiMatrix);
    }
}

void TransformTool::rotate(Mesh* m, float rangle, float rx, float ry, float rz)
{
	// rx ... vector normalisieren

    float rMatrix[3][4] = {{cos(rangle)+(rx*rx)*(1-cos(rangle)), (rx*ry)*(1-cos(rangle))-rz*sin(rangle), (rx*rz)*(1-cos(rangle))+ry*sin(rangle), 0.0f},
                           {(rx*ry)*(1-cos(rangle))+rz*sin(rangle), cos(rangle)+(ry*ry)*(1-cos(rangle)), (ry*rz)*(1-cos(rangle))-rx*sin(rangle), 0.0f},
                           {(rz*rx)*(1-cos(rangle))-ry*sin(rangle), (rz*ry)*(1-cos(rangle))+rx*sin(rangle), cos(rangle)+(rx*rx)*(1-cos(rangle)), 0.0f}};
                           
    float riMatrix[3][3] = {{rMatrix[0][0], rMatrix[0][1], rMatrix[0][2]},
                            {rMatrix[1][0], rMatrix[1][1], rMatrix[1][2]},
                            {rMatrix[2][0], rMatrix[2][1], rMatrix[2][2]}};
    
                           
	for(unsigned int i = 0; i < m->getNumberOfVertices(); ++i)
	{
		Vertex* pVertex = &m->getVertex(i);

        calculateVectorWithMatrix(pVertex, rMatrix, riMatrix);
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
        
        calculateVectorWithMatrix(pVertex, sMatrix, siMatrix);
        
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
{   float rsx, rsy, rsz;
    rsx = rsy = rsz = 0.0f;
    if(axis == 'x')
    {
        rsx = val/m->getWidth();
        rsy = rsz = rsx;
    }
    else if(axis == 'y')
    {
        rsy = val/m->getHeight();
        rsx = rsz = rsy;
    }
    else if(axis == 'z')
    {
        rsz = val/m->getLength();
        rsx = rsy = rsz;
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

void TransformTool::calculateVectorWithMatrix(Vertex* vec, float matrix[3][4], float inverseTransposedMatrix[3][3])
{
	vec->position[0] = vec->position[0]*matrix[0][0] + vec->position[1]*matrix[0][1] + vec->position[2]*matrix[0][2] + matrix[0][3];
    vec->position[1] = vec->position[0]*matrix[1][0] + vec->position[1]*matrix[1][1] + vec->position[2]*matrix[1][2] + matrix[1][3];
    vec->position[2] = vec->position[0]*matrix[2][0] + vec->position[1]*matrix[2][1] + vec->position[2]*matrix[2][2] + matrix[2][3];

	//normale, tangenten, bitangen inverse transpose
    
    vec->normal[0] = vec->normal[0]*inverseTransposedMatrix[0][0] + vec->normal[1]*inverseTransposedMatrix[0][1] + vec->normal[2]*inverseTransposedMatrix[0][2];
    vec->normal[1] = vec->normal[0]*inverseTransposedMatrix[1][0] + vec->normal[1]*inverseTransposedMatrix[1][1] + vec->normal[2]*inverseTransposedMatrix[1][2];
    vec->normal[2] = vec->normal[0]*inverseTransposedMatrix[2][0] + vec->normal[1]*inverseTransposedMatrix[2][1] + vec->normal[2]*inverseTransposedMatrix[2][2];

	vec->bitangent[0] = vec->bitangent[0]*inverseTransposedMatrix[0][0] + vec->bitangent[1]*inverseTransposedMatrix[0][1] + vec->bitangent[2]*inverseTransposedMatrix[0][2];
    vec->bitangent[1] = vec->bitangent[0]*inverseTransposedMatrix[1][0] + vec->bitangent[1]*inverseTransposedMatrix[1][1] + vec->bitangent[2]*inverseTransposedMatrix[1][2];
    vec->bitangent[2] = vec->bitangent[0]*inverseTransposedMatrix[2][0] + vec->bitangent[1]*inverseTransposedMatrix[2][1] + vec->bitangent[2]*inverseTransposedMatrix[2][2];
    
	vec->tangent[0] = vec->tangent[0]*inverseTransposedMatrix[0][0] + vec->tangent[1]*inverseTransposedMatrix[0][1] + vec->tangent[2]*inverseTransposedMatrix[0][2];
    vec->tangent[1] = vec->tangent[0]*inverseTransposedMatrix[1][0] + vec->tangent[1]*inverseTransposedMatrix[1][1] + vec->tangent[2]*inverseTransposedMatrix[1][2];
    vec->tangent[2] = vec->tangent[0]*inverseTransposedMatrix[2][0] + vec->tangent[1]*inverseTransposedMatrix[2][1] + vec->tangent[2]*inverseTransposedMatrix[2][2];
    
    normalize(vec);
}
void TransformTool::normalize(Vertex* vec)
{
    float tmpLength = 0.0f;
    float length = 0.0f;
 
    // normal
    tmpLength = sqrtf(vec->normal[0] * vec->normal[0] +
        vec->normal[1] * vec->normal[1] +
        vec->normal[2] * vec->normal[2]);

    if(tmpLength == 0.0f)
        tmpLength = 0.0f;
    else
        length = 1.0f / tmpLength;

    vec->normal[0] *= length;
    vec->normal[1] *= length;
    vec->normal[2] *= length;
    
    // tangent
    tmpLength = sqrtf(vec->tangent[0] * vec->tangent[0] +
        vec->tangent[1] * vec->tangent[1] +
        vec->tangent[2] * vec->tangent[2]);

    if(tmpLength == 0.0f)
        length = 0.0f;
    else
        length = 1.0f / tmpLength;

    vec->tangent[0] *= length;
    vec->tangent[1] *= length;
    vec->tangent[2] *= length;

    // bitangent
    tmpLength = sqrtf(vec->bitangent[0] * vec->bitangent[0] +
        vec->bitangent[1] * vec->bitangent[1] +
        vec->bitangent[2] * vec->bitangent[2]);

    if(tmpLength == 0.0f)
        length = 0.0f;
    else
        length = 1.0f / tmpLength;

    vec->bitangent[0] *= length;
    vec->bitangent[1] *= length;
    vec->bitangent[2] *= length;
    
    
    
}
