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

#include "TesterTool.h"
#include "WizUtils.h"

using namespace WizUtils;
using namespace MeshTester;

TesterTool::TesterTool(const char* actual, const char* expected)
{
    loadMesh(actual, m_attributesActual);

    loadMesh(expected, m_attributesExpected);

}

void TesterTool::loadMesh(const std::string &xmlPath, std::vector<Attribute>& attribs)
{
    XmlParser xml;
    xml.loadFile(xmlPath);

    xml.pushTag("Mesh");
    {
        int numVertices = xml.getAttribute("Vertices", "count", 0);
        int numAtributes = xml.getAttribute("Vertices", "attributes", 0);

        xml.pushTag("Vertices");
        {
            for(int i = 0; i < numAtributes; ++i)
            {
                Attribute attrib;
                attrib.name = xml.getAttribute("Attribute", "name", "", i).c_str();
                attrib.size = xml.getAttribute("Attribute", "size", 0, i);
                attrib.values = new float[numVertices * attrib.size];
                attribs.push_back(attrib);
            }
        }
        xml.popTag();
    }
    xml.popTag();

}

void TesterTool::start()
{
    // test attributes

    for(unsigned int i = 0; i < m_attributesActual.size(); ++i)
    {

    }

    // test indices

}

void TesterTool::printResults()
{

}

int TesterTool::compare(int n, float* array_a, float* array_b, float epsilon)
{
    return 0;
}

int TesterTool::compare(int n, unsigned char* array_a, unsigned char* array_b)
{
    return 0;
}

