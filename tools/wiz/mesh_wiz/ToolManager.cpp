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
#include "ToolManager.h"
#include <cmath>

using namespace assembly3d;
using namespace assembly3d::wiz;

ToolManager::ToolManager(Mesh* mesh, bool verbose) 
    :
      m_mesh(mesh),
      m_verboseOutput(verbose),
      m_convertTool(new ConvertTool()),
      m_transformTool(new TransformTool()),
      m_attributeTool(new AttributeTool()),
      m_optimizeTool(new OptimizeTool()),
      m_frontFaceTool(new FrontFaceTool()),
      m_textureTool(new BakeTool()),
      m_meshTool(new MeshTool())
{
    
}

ToolManager::~ToolManager()
{
    SAFE_DELETE(m_convertTool);
    SAFE_DELETE(m_transformTool);
    SAFE_DELETE(m_attributeTool);
    SAFE_DELETE(m_optimizeTool);
    SAFE_DELETE(m_frontFaceTool);
    SAFE_DELETE(m_textureTool);
    SAFE_DELETE(m_meshTool);
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

void ToolManager::translate(float tx, float ty, float tz, bool transformTexCoords)
{
    if(m_verboseOutput) 
        std::cout << "Translating mesh (x=" << tx << ", y=" << ty << ", z=" << tz << ")" << std::endl;
    
    //    m_transformTool->translate(m_mesh, tx, ty, tz);

    Mesh::Attribute attrib;

    if(transformTexCoords)
    {
        attrib = m_mesh->getAttribute(Mesh::TEXCOORD);
        m_transformTool->translate(&attrib, tx, ty, tz, false);
        if(m_mesh->hasTangents() || m_mesh->hasBitangents())
        {
            m_mesh->generateTangents();
        }
    }
    else
    {
        attrib = m_mesh->getAttribute(Mesh::POSITION);
        //    m_mesh->printAttribute(attrib);
        m_transformTool->translate(&attrib, tx, ty, tz, false);
        if(m_mesh->hasNormals())
        {
            attrib = m_mesh->getAttribute(Mesh::NORMAL);
            m_transformTool->translate(&attrib, tx, ty, tz, true);
        }
        if(m_mesh->hasTangents())
        {
            attrib = m_mesh->getAttribute(Mesh::TANGENT);
            m_transformTool->translate(&attrib, tx, ty, tz, true);
        }
        if(m_mesh->hasBitangents())
        {
            attrib = m_mesh->getAttribute(Mesh::BITANGENT);
            m_transformTool->translate(&attrib, tx, ty, tz, true);
        }
    }

}

void ToolManager::rotate(float rangle, float rx, float ry, float rz, bool transformTexCoords)
{
    if(m_verboseOutput) 
        std::cout << "Rotating mesh (angle=" << rangle << ", x=" << rx << ", y=" << ry << ", z=" << rz << ")" << std::endl;
    
    //    m_transformTool->rotate(m_mesh, rangle, rx, ry, rz);

    Mesh::Attribute attrib;

    if(transformTexCoords)
    {
        attrib = m_mesh->getAttribute(Mesh::TEXCOORD);
        m_transformTool->rotate(&attrib, rangle, rx, ry, rz);
        if(m_mesh->hasTangents() || m_mesh->hasBitangents())
        {
            m_mesh->generateTangents();
        }
    }
    else
    {

        attrib = m_mesh->getAttribute(Mesh::POSITION);
        //    m_mesh->printAttribute(attrib);
        m_transformTool->rotate(&attrib, rangle, rx, ry, rz);
        if(m_mesh->hasNormals())
        {
            attrib = m_mesh->getAttribute(Mesh::NORMAL);
            m_transformTool->rotate(&attrib, rangle, rx, ry, rz, true);
        }
        if(m_mesh->hasTangents())
        {
            attrib = m_mesh->getAttribute(Mesh::TANGENT);
            m_transformTool->rotate(&attrib, rangle, rx, ry, rz, true);
        }
        if(m_mesh->hasBitangents())
        {
            attrib = m_mesh->getAttribute(Mesh::BITANGENT);
            m_transformTool->rotate(&attrib, rangle, rx, ry, rz, true);
        }
    }
}

void ToolManager::scale(float sx, float sy, float sz, bool transformTexCoords)
{
    if(m_verboseOutput) 
        std::cout << "Scaling mesh (x=" << sx << ", y=" << sy << ", z=" << sz << ")" << std::endl;

    //    m_transformTool->scale(m_mesh, sx, sy, sz);

    Mesh::Attribute attrib;
    if(transformTexCoords)
    {
        attrib = m_mesh->getAttribute(Mesh::TEXCOORD);
        m_transformTool->scale(&attrib, sx, sy, sz);
        if(m_mesh->hasTangents() || m_mesh->hasBitangents())
        {
            m_mesh->generateTangents();
        }
    }
    else
    {

        attrib = m_mesh->getAttribute(Mesh::POSITION);
        m_transformTool->scale(&attrib, sx, sy, sz);
        if(m_mesh->hasNormals())
        {
            attrib = m_mesh->getAttribute(Mesh::NORMAL);
            m_transformTool->scale(&attrib, sx, sy, sz, true);
        }
        if(m_mesh->hasTangents())
        {
            attrib = m_mesh->getAttribute(Mesh::TANGENT);
            m_transformTool->scale(&attrib, sx, sy, sz, true);
        }
        if(m_mesh->hasBitangents())
        {
            attrib = m_mesh->getAttribute(Mesh::BITANGENT);
            m_transformTool->scale(&attrib, sx, sy, sz, true);
        }
    }
}

void ToolManager::remapAxes(const char* newX, const char* newY, const char* newZ)
{
	if(m_verboseOutput)
	{
		std::cout << "Remapping main axes: ";
		std::cout << "x -> " << newX << ", ";
		std::cout << "y -> " << newY << ", ";
		std::cout << "z -> " << newZ << std::endl;
	}
	std::vector<std::string> components;
	components.push_back(newX);
	components.push_back(newY);
	components.push_back(newZ);
	
	std::vector<float*> axes;
	float tmp[3] = {0.0f};
	axes.push_back(tmp);
	axes.push_back(tmp);
	axes.push_back(tmp);
	
	float col1[3] = {1.0f, 0.0f, 0.0f};
	float col2[3] = {0.0f, 1.0f, 0.0f};
	float col3[3] = {0.0f, 0.0f, 1.0f};
	
	bool malformed = false;
	for (size_t i = 0; i < components.size(); ++i) {
		std::string component = components[i];
		char axis;
		float factor = 1.0f;
		
		if (component.size() > 2 || component.empty())
		{
			malformed = true;
			break;
		}
		else if (component.size() == 2)
		{
			if (component.at(0) == '-')
			{
				factor = -1.0f;
			}
			else if (component.at(0) != '+')
			{
				malformed = true;
				break;
			}
			
			axis = component.at(1);
		}
		else
		{
			axis = component.at(0);
		}
		
		if (axis == 'x' || axis == 'X')
		{
			axes[i][0] = col1[0] * factor;
			axes[i][1] = col1[1] * factor;
			axes[i][2] = col1[2] * factor;
		}
		else if (axis == 'y' || axis == 'Y')
		{
			axes[i][0] = col2[0] * factor;
			axes[i][1] = col2[1] * factor;
			axes[i][2] = col2[2] * factor;
		}
		else if (axis == 'z' || axis == 'Z')
		{
			axes[i][0] = col3[0] * factor;
			axes[i][1] = col3[1] * factor;
			axes[i][2] = col3[2] * factor;
		}
		else
		{
			malformed = true;
			break;
		}
	}
	if(malformed == false)
	{
		Mesh::Attribute pos = m_mesh->getAttribute(Mesh::POSITION);
		m_transformTool->remapAxes(&pos, axes[0], axes[1], axes[2]);
		
		if(m_mesh->hasNormals())
		{
			Mesh::Attribute normal = m_mesh->getAttribute(Mesh::NORMAL);
			m_transformTool->remapAxes(&normal, axes[0], axes[1], axes[2], true);
		}
		if(m_mesh->hasTexCoords())
		{
			Mesh::Attribute texCoord = m_mesh->getAttribute(Mesh::TEXCOORD);
			m_transformTool->remapAxes(&texCoord, axes[0], axes[1], axes[2], true);
		}
		if(m_mesh->hasTangents())
		{
			Mesh::Attribute tangent = m_mesh->getAttribute(Mesh::TEXCOORD);
			m_transformTool->remapAxes(&tangent, axes[0], axes[1], axes[2], true);
		}
		if(m_mesh->hasBitangents())
		{
			Mesh::Attribute bitangent = m_mesh->getAttribute(Mesh::TEXCOORD);
			m_transformTool->remapAxes(&bitangent, axes[0], axes[1], axes[2], true);
		}	
	}
	else 
	{
		std::cout << "unrecognized --axes optio value. skipping..." << std::endl;
	}

}
void ToolManager::resize(float rsx, float rsy, float rsz, bool transformTexCoords)
{
    if(m_verboseOutput) 
        std::cout << "Resizing mesh (x=" << rsx << ", y=" << rsy << ", z=" << rsz << ")" << std::endl;

    //    m_transformTool->resize(m_mesh, rsx, rsy, rsz);

    Mesh::Attribute attrib;
    if(transformTexCoords)
    {
        attrib = m_mesh->getAttribute(Mesh::TEXCOORD);
        m_transformTool->resize(&attrib, rsx, rsy, rsz, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength());
        if(m_mesh->hasTangents() || m_mesh->hasBitangents())
        {
            m_mesh->generateTangents();
        }
    }
    else
    {

        attrib = m_mesh->getAttribute(Mesh::POSITION);
        m_transformTool->resize(&attrib, rsx, rsy, rsz, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength());
        if(m_mesh->hasNormals())
        {
            attrib = m_mesh->getAttribute(Mesh::NORMAL);
            m_transformTool->resize(&attrib, rsx, rsy, rsz, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength(), true);
        }
        if(m_mesh->hasTangents())
        {
            attrib = m_mesh->getAttribute(Mesh::TANGENT);
            m_transformTool->resize(&attrib, rsx, rsy, rsz, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength(), true);
        }
        if(m_mesh->hasBitangents())
        {
            attrib = m_mesh->getAttribute(Mesh::BITANGENT);
            m_transformTool->resize(&attrib, rsx, rsy, rsz, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength(), true);
        }
    }
}

void ToolManager::resize(const char* axis, float val, bool transformTexCoords)
{
    if(m_verboseOutput) 
        std::cout << "Resizing mesh on axis=" << axis << " to value=" << val << std::endl;

    //    m_transformTool->resize(m_mesh, axis[0], val);

    Mesh::Attribute attrib;

    if(transformTexCoords)
    {
        attrib = m_mesh->getAttribute(Mesh::TEXCOORD);
        m_transformTool->resize(&attrib, axis[0], val, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength());
        if(m_mesh->hasTangents() || m_mesh->hasBitangents())
        {
            m_mesh->generateTangents();
        }
    }
    else
    {

        attrib = m_mesh->getAttribute(Mesh::POSITION);
        m_transformTool->resize(&attrib, axis[0], val, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength());
        if(m_mesh->hasNormals())
        {
            attrib = m_mesh->getAttribute(Mesh::NORMAL);
            m_transformTool->resize(&attrib, axis[0], val, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength(), true);
        }
        if(m_mesh->hasTangents())
        {
            attrib = m_mesh->getAttribute(Mesh::TANGENT);
            m_transformTool->resize(&attrib, axis[0], val, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength(), true);
        }
        if(m_mesh->hasBitangents())
        {
            attrib = m_mesh->getAttribute(Mesh::BITANGENT);
            m_transformTool->resize(&attrib, axis[0], val, m_mesh->getWidth(), m_mesh->getHeight(), m_mesh->getLength(), true);
        }
    }
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


void ToolManager::center(int axisX, int axisY, int axisZ, bool transformTexCoords)
{
    if(m_verboseOutput)
        std::cout << "Centering mesh" << std::endl;

    //    m_transformTool->center(m_mesh, axisX, axisY, axisZ);

    float centerX, centerY, centerZ;
    centerX = centerY = centerZ = 0.0f;
    m_mesh->getCenter(centerX, centerY, centerZ);

    Mesh::Attribute attrib;
    if(transformTexCoords)
    {
        attrib = m_mesh->getAttribute(Mesh::TEXCOORD);
        m_transformTool->center(&attrib, axisX, axisY, axisZ, centerX, centerY, centerZ);
        if(m_mesh->hasTangents() || m_mesh->hasBitangents())
        {
            m_mesh->generateTangents();
        }
    }
    else
    {

        attrib = m_mesh->getAttribute(Mesh::POSITION);
        m_transformTool->center(&attrib, axisX, axisY, axisZ, centerX, centerY, centerZ);
        if(m_mesh->hasNormals())
        {
            attrib = m_mesh->getAttribute(Mesh::NORMAL);
            m_transformTool->center(&attrib, axisX, axisY, axisZ, centerX, centerY, centerZ, true);
        }
        if(m_mesh->hasTangents())
        {
            attrib = m_mesh->getAttribute(Mesh::TANGENT);
            m_transformTool->center(&attrib, axisX, axisY, axisZ, centerX, centerY, centerZ, true);
        }
        if(m_mesh->hasBitangents())
        {
            attrib = m_mesh->getAttribute(Mesh::BITANGENT);
            m_transformTool->center(&attrib, axisX, axisY, axisZ, centerX, centerY, centerZ, true);
        }
    }
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

    Mesh::AttributeType attrib;

    if(std::string(attributeName).compare("position") == 0)
    {
        attrib = Mesh::POSITION;
    }
    else if(std::string(attributeName).compare("normal") == 0)
    {
        attrib = Mesh::NORMAL;
    }
    else if(std::string(attributeName).compare("texture") == 0)
    {
        attrib = Mesh::TEXCOORD;
    }
    else if(std::string(attributeName).compare("tangent") == 0)
    {
        attrib = Mesh::TANGENT;
    }
    else if(std::string(attributeName).compare("bitangent") == 0)
    {
        attrib = Mesh::BITANGENT;
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

void ToolManager::flipWinding()
{
    if(m_verboseOutput)
        std::cout << "Flipping winding" << std::endl;
	
    m_frontFaceTool->changeWinding(m_mesh);
	
}
bool ToolManager::makeNormalsConsistent()
{
    bool modelChanged = false;
    if(m_verboseOutput)
        std::cout << "Testing normal conistancy" << std::endl;

    std::string resMsg;

    modelChanged = m_frontFaceTool->makeConsistent(m_mesh, resMsg);

    if(m_verboseOutput)
        std::cout << resMsg << std::endl;

    return modelChanged;
}

bool ToolManager::checkFrontFaceConsistenty(int& numOutwards, int& numInwards)
{
    std::vector<int> vertsOutwards;
    std::vector<int> vertsInwards;

    bool consistent = m_frontFaceTool->isConsistent(m_mesh, vertsOutwards, vertsInwards);
    numOutwards = (int)vertsOutwards.size();
    numInwards = (int)vertsInwards.size();

    if(consistent)
        return true;
    else
        return false;

}

int ToolManager::checkBakeable()
{
	Mesh::Attribute attrib = m_mesh->getAttribute(Mesh::TEXCOORD);
    return m_textureTool->isInBounds(&attrib, m_mesh->getNumberOfVertices());
}

int ToolManager::checkUVOverlapping()
{
    return m_textureTool->checkUVOverlapping(m_mesh);
}

void ToolManager::mergeMeshes(Mesh *second)
{
    m_meshTool->merge(m_mesh, second);
}
