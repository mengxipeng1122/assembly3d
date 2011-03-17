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
#include <fstream>

using namespace WizUtils;
using namespace MeshTest;
using namespace std;

TesterTool::TesterTool()
{
}

void TesterTool::loadMesh(const string& xmlFile,
                          const string& binaryFile,
                          vector<Attribute>& attribs,
                          vector<Group>& groups)
{
    XmlParser xml;
    xml.loadFile(xmlFile);

    ifstream fin(binaryFile.c_str(), ios::binary);

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
                attrib.count = numVertices * attrib.size;
                attrib.values = new float[attrib.count];

                for(int j = 0; j < numVertices; ++j)
                {
                    for(int k = 0; k < attrib.size; ++k)
                    {
                        float val = 0.0f;

                        fin.read((char *)(&val), sizeof(val));

                        attrib.values[j*attrib.size + k] = val;
                    }
                }

                attribs.push_back(attrib);
            }
        }
        xml.popTag();

        int numGroups = xml.getAttribute("Triangles", "groups", 0);
        string typeIndices = xml.getAttribute("Triangles", "type", "");

        xml.pushTag("Triangles");
        {
            for(int i = 0; i < numGroups; ++i)
            {
                Group group;
                group.type = typeIndices.c_str();
                group.name = xml.getAttribute("Group", "name", "", i).c_str();
                group.count = xml.getAttribute("Group", "count", 0, i);

                int numBytes = 0;

                if(string(group.type).compare("UNSIGNED_BYTE") == 0)
                {
                    numBytes = group.count * 3 * sizeof(unsigned char);
                }
                else if(string(group.type).compare("UNSIGNED_SHORT") == 0)
                {
                    numBytes = group.count * 3 * sizeof(unsigned short);
                }
                else if(string(group.type).compare("UNSIGNED_INT") == 0)
                {
                    numBytes = group.count * 3 * sizeof(unsigned int);
                }

                group.numBytes = numBytes;
                group.bytes = new unsigned char[numBytes];

                for(int j = 0; j < group.count*3; ++j)
                {
                    unsigned char val = 0;

                    fin.read((char *)(&val), sizeof(val));

                    group.bytes[j] = val;
                }

                groups.push_back(group);

            }
        }
        xml.popTag();
    }
    xml.popTag();
    xml.clear();
    fin.close();

}

void TesterTool::compare(const char* actual,
                         const char* actualBinary,
                         const char* expected,
                         const char* expectedBinary,
                         float epsilon)
{
    vector<Attribute> attributesActual;
    vector<Attribute> attributesExpected;
    vector<Group> groupsActual;
    vector<Group> groupsExpected;

    loadMesh(actual, actualBinary, attributesActual, groupsActual);
    loadMesh(expected, expectedBinary, attributesExpected, groupsExpected);

    bool attribsPass = false;
    bool groupsPass = false;

    // compare attributes
    if(attributesActual.size() == attributesExpected.size())
    {
        for(unsigned int i = 0; i < attributesExpected.size(); ++i)
        {
            const Attribute& attribA = attributesActual[i];
            const Attribute& attribE = attributesExpected[i];
            if(attribA.count == attribE.count)
            {
                if(compare(attribE.count,
                           attribA.values,
                           attribE.values,
                           epsilon) == 0)
                {
                    attribsPass = true;
                }
                else
                {
                    attribsPass = false;
                    break;
                }
            }
            else
            {
                attribsPass = false;
                break;
            }
        }
    }

    // compare groups
    if(groupsActual.size() == groupsExpected.size())
    {
        for(unsigned int i = 0; i < groupsExpected.size(); ++i)
        {
            const Group& groupA = groupsActual[i];
            const Group& groupE = groupsExpected[i];
            if(groupA.numBytes == groupE.numBytes)
            {
                if(compare(groupE.numBytes,
                           groupA.bytes,
                           groupE.bytes) == 0)
                {
                    groupsPass = true;
                }
                else
                {
                    groupsPass = false;
                    break;
                }
            }
            else
            {
                groupsPass = false;
                break;
            }
        }
    }

    // print result
    if(attribsPass && groupsPass)
    {
        cout << "Passed!" << endl;
    }
    else
    {
        cout << "Failed!" << endl;
    }

    // delete allocated memory
    clear(attributesActual, groupsActual);
    clear(attributesExpected, groupsExpected);

}

void TesterTool::clear(vector<Attribute>& attribs,
                       vector<Group>& groups)
{
    for(unsigned int i = 0; i < attribs.size(); ++i)
    {
        delete[] attribs[i].values;
    }
    for(unsigned int i = 0; i < groups.size(); ++i)
    {
        delete[] groups[i].bytes;
    }
}

int TesterTool::compare(int n, float* array_a, float* array_e, float epsilon)
{
    for(int i = 0; i < n; ++i)
    {
        if(array_a[i] < array_e[i] - epsilon/2 ||
           array_a[i] > array_e[i] + epsilon/2 )
        {
            return -1;
        }
    }
    return 0;
}

int TesterTool::compare(int n, unsigned char* array_a, unsigned char* array_e)
{
    return memcmp(array_a, array_e, n);
}