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

void TransformTool::translate(Mesh::Attribute *attribute, float tx, float ty,
                              float tz, bool inverseTranspose)
{
    float matrix[3][4] = {{1.0f,0.0f,0.0f,tx},
                         {0.0f,1.0f,0.0f,ty},
                         {0.0f,0.0f,1.0f,tz}};

    if(inverseTranspose)
    {
        matrix[0][3] = 0.0f;
        matrix[1][3] = 0.0f;
        matrix[2][3] = 0.0f;
    }
	transform(attribute, matrix);
}

void TransformTool::rotate(Mesh::Attribute *attribute, float rangle, float rx,
                           float ry, float rz, bool inverseTranspose)
{
    // normalize rotation axis
    float rvec[3] = {rx, ry, rz};
    normalize(rvec, 3);
    rx = rvec[0];
    ry = rvec[1];
    rz = rvec[2];

    float s = sinf(2.0f*PIf*rangle/360.0f);
    float c = cosf(2.0f*PIf*rangle/360.0f);
    float t = 1.0f - c;

    float matrix[3][4] = {{rx*rx*t + c, rx*ry*t - rz*s, rx*rz*t + ry*s, 0.0f},
                           {rx*rz*t + ry*s, ry*ry*t + c, ry*rz*t - rx*s, 0.0f},
                           {rz*rx*t - ry*s, rz*ry*t + rx*s, rz*rz*t + c, 0.0f}};

    transform(attribute, matrix);
}

void TransformTool::scale(Mesh::Attribute* attribute, float sx, float sy,
                          float sz, bool inverseTranspose)
{
    float matrix[3][4] = {{sx,0.0f,0.0f,0.0f},
                         {0.0f,sy,0.0f,0.0f},
                         {0.0f,0.0f,sz,0.0f}};

    if(inverseTranspose)
    {
        matrix[0][0] = 1.0f/sx;
        matrix[1][1] = 1.0f/sy;
        matrix[2][2] = 1.0f/sz;
    }
	transform(attribute, matrix);
}

void TransformTool::resize(Mesh::Attribute* attribute, float width, float height,
                           float length, float mWidth, float mHeight,float mLength,
                           bool inverseTranspose)
{
    float rsx = width/mWidth;
    float rsy = height/mHeight;
    float rsz = length/mLength;

    scale(attribute, rsx, rsy, rsz, inverseTranspose);
}

void TransformTool::resize(Mesh::Attribute* attribute, const char axis,
                           float val, float mWidth, float mHeight,float mLength,
                           bool inverseTranspose)
{
    float rsx, rsy, rsz;
    rsx = rsy = rsz = 0.0f;
    if(axis == 'x')
    {
        rsx = val/mWidth;
        rsy = rsx;
        rsz = rsx;
    }
    else if(axis == 'y')
    {
        rsy = val/mHeight;
        rsx = rsy;
        rsz = rsy;
    }
    else if(axis == 'z')
    {
        rsz = val/mLength;
        rsx = rsz;
        rsy = rsz;
    }

    scale(attribute, rsx, rsy, rsz, inverseTranspose);
}

void TransformTool::center(Mesh::Attribute* attribute, int ax, int ay, int az,
                           float centerX, float centerY, float centerZ,
                           bool inverseTranspose)
{
    float tX, tY, tZ;
    tX = tY = tZ = 0.0f;
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

    translate(attribute, tX, tY, tZ, inverseTranspose);
}

void TransformTool::remapAxes(Mesh::Attribute* attribute, float matrixCol1[3],
							  float matrixCol2[3], float matrixCol3[3], 
							  bool inverseTranspose)
{
    float matrix[3][4] = {{matrixCol1[0],matrixCol2[0],matrixCol3[0],0.0f},
						  {matrixCol1[1],matrixCol2[1],matrixCol3[1],0.0f},
						  {matrixCol1[2],matrixCol2[2],matrixCol3[2],0.0f}};

	transform(attribute, matrix);
}

void TransformTool::normalize(float* vector, int size)
{
    float length = 0.0f;
    float squareSum = 0.0f;
    for(int i = 0; i < size; ++i)
    {
        squareSum += vector[i]*vector[i];
    }
    length = sqrtf(squareSum);

    if(length == 0.0f)
        length = 0.0f;
    else
        length = 1.0f / length;

    for(int i = 0; i < size; ++i)
    {
        vector[i] *= length;
    }
}


void TransformTool::transform(Mesh::Attribute *attribute, float matrix[3][4])
{
    float* data = attribute->data;
	
    // iterating over vertices
    for(int i = 0; i < attribute->count/4; ++i)
    {
		float x,y,z,w;
		x = data[i*4+0]; y = data[i*4+1]; z = data[i*4+2], w = data[i*4+3];
		data[i*4+0] = x*matrix[0][0] + y*matrix[0][1] + z*matrix[0][2] + w*matrix[0][3];
		data[i*4+1] = x*matrix[1][0] + y*matrix[1][1] + z*matrix[1][2] + w*matrix[1][3];
		data[i*4+2] = x*matrix[2][0] + y*matrix[2][1] + z*matrix[2][2] + w*matrix[2][3];
    }
	
	if(attribute->type == Mesh::NORMAL ||
	   attribute->type == Mesh::TANGENT ||
	   attribute->type == Mesh::BITANGENT)
	{
		for(int i = 0; i < attribute->count/4; ++i)
		{
			normalize(&data[i*4], attribute->size);
		}
	}
	
	
}

