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

#include "ToolManager.h"
#include <cmath>

using namespace assembly3d;
using namespace assembly3d::wiz::mesh;

ToolManager::ToolManager(Mesh* mesh, bool verbose) 
:
m_mesh(mesh),
m_verboseOutput(verbose),
m_convertTool(new ConvertTool()),
m_transformTool(new TransformTool()),
m_attributeTool(new AttributeTool()),
m_optimizeTool(new OptimizeTool()),
m_frontFaceTool(new FrontFaceTool())
{
    
}

ToolManager::~ToolManager()
{
    SAFE_DELETE(m_convertTool)
    SAFE_DELETE(m_transformTool)
    SAFE_DELETE(m_attributeTool)
    SAFE_DELETE(m_optimizeTool)
    SAFE_DELETE(m_frontFaceTool)
}
bool ToolManager::convertIndexType(const char* type)
{
    if(m_verboseOutput) 
        std::cout << "Converting index type to 'unsigned " << type << "'" << std::endl;
    
    bool result = false;

    std::string stype = type;
    if(stype.compare("int") == 0)
    {
        m_convertTool->convertIndicesToUnsignedInt(m_mesh);
        result = true;
    }    
    else if(stype.compare("short") == 0)
    {
		int sizeInt = sizeof(unsigned short)*8;
		if(m_mesh->getNumberOfVertices() > (pow(2.0,sizeInt)))
        {
            result = false;
        }
        else
        {
            m_convertTool->convertIndicesToUnsignedShort(m_mesh);
            result = true;
        }
        
    }
    else if(stype.compare("byte") == 0)
    {
        int sizeInt = sizeof(unsigned char)*8;
		if(m_mesh->getNumberOfVertices() > (pow(2.0,sizeInt)))
        {
            result = false;
        }
        else
        {
            m_convertTool->convertIndicesToUnsignedByte(m_mesh);
            result = true;
        }
    }
    return result;
}

void ToolManager::translate(float tx, float ty, float tz)
{
    if(m_verboseOutput) 
        std::cout << "Translating mesh (x=" << tx << ", y=" << ty << ", z=" << tz << ")" << std::endl;
    
    m_transformTool->translate(m_mesh, tx, ty, tz);
}

void ToolManager::rotate(float rangle, float rx, float ry, float rz)
{
    if(m_verboseOutput) 
        std::cout << "Rotating mesh (angle=" << rangle << ", x=" << rx << ", y=" << ry << ", z=" << rz << ")" << std::endl;
    
    m_transformTool->rotate(m_mesh, rangle, rx, ry, rz);
}

void ToolManager::scale(float sx, float sy, float sz)
{
    if(m_verboseOutput) 
        std::cout << "Scaling mesh (x=" << sx << ", y=" << sy << ", z=" << sz << ")" << std::endl;
        
    m_transformTool->scale(m_mesh, sx, sy, sz);
}

void ToolManager::resize(float rsx, float rsy, float rsz)
{
    if(m_verboseOutput) 
        std::cout << "Resizing mesh (x=" << rsx << ", y=" << rsy << ", z=" << rsz << ")" << std::endl;

    m_transformTool->resize(m_mesh, rsx, rsy, rsz);   
}

void ToolManager::resize(const char* axis, float val)
{
    if(m_verboseOutput) 
        std::cout << "Resizing mesh on axis=" << axis << " to value=" << val << std::endl;

    m_transformTool->resize(m_mesh, axis[0], val);   
}

void ToolManager::generateNormals()
{
    if(m_verboseOutput) 
        std::cout << "Generating normals" << std::endl;
        
    m_attributeTool->addNormals(m_mesh);
}

void ToolManager::generateTangents()
{
    if(m_verboseOutput) 
        std::cout << "Generating tangents" << std::endl;

    m_attributeTool->addTangents(m_mesh);
}

void ToolManager::generateBitangents()
{
    if(m_verboseOutput) 
        std::cout << "Generating bitangents" << std::endl;

    m_attributeTool->addBitangents(m_mesh);
}

void ToolManager::removeNormals()
{
    if(m_verboseOutput) 
        std::cout << "Removing normals" << std::endl;

    m_attributeTool->removeNormals(m_mesh);
}

void ToolManager::removeTangents()
{
    if(m_verboseOutput) 
        std::cout << "Removing tangents" << std::endl;

    m_attributeTool->removeTangents(m_mesh);
}

void ToolManager::removeBitangents()
{
    if(m_verboseOutput) 
        std::cout << "Removing bitangents" << std::endl;

    m_attributeTool->removeBitangents(m_mesh);
}


void ToolManager::center(int axisX, int axisY, int axisZ)
{
    if(m_verboseOutput) 
        std::cout << "Centering mesh" << std::endl;

    m_transformTool->center(m_mesh, axisX, axisY, axisZ);
}

void ToolManager::stitch()
{
    if(m_verboseOutput)
        std::cout << "Stitching vertices" << std::endl;

    m_optimizeTool->stitch(m_mesh);
}

void ToolManager::stitchEps(const char* attributeName, float epsilon)
{
    if(m_verboseOutput)
        std::cout << "Stitching vertices. Comparing " << attributeName << " with an epsilon of " << epsilon << std::endl;

    OptimizeTool::Attribute attrib;

    if(std::string(attributeName).compare("postion") == 0)
    {
        attrib = OptimizeTool::POSITION;
    }
    else if(std::string(attributeName).compare("normal") == 0)
    {
        attrib = OptimizeTool::NORMAL;
    }
    else if(std::string(attributeName).compare("texture") == 0)
    {
        attrib = OptimizeTool::TEXCOORD;
    }
    else if(std::string(attributeName).compare("tangent") == 0)
    {
        attrib = OptimizeTool::TANGENT;
    }
    else if(std::string(attributeName).compare("bitangent") == 0)
    {
        attrib = OptimizeTool::BITANGENT;
    }
    else
    {
        std::cout << "No suitable attribute given, stiching without epsilon" << std::endl;
        m_optimizeTool->stitch(m_mesh);
    }
    m_optimizeTool->stitch(m_mesh, attrib, epsilon);

}

void ToolManager::flip()
{
    if(m_verboseOutput)
        std::cout << "Flipping Front-Face" << std::endl;

    m_frontFaceTool->flip(m_mesh);
}

bool ToolManager::testNormalConsitancy()
{
    bool modelChanged = false;
    if(m_verboseOutput)
        std::cout << "Testing normal conistancy" << std::endl;

    std::string resMsg;

    modelChanged = m_frontFaceTool->testNormalConsitency(m_mesh, resMsg);

    if(m_verboseOutput)
        std::cout << resMsg << std::endl;

    return modelChanged;
}
