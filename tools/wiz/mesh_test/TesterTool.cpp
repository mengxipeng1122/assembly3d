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
#include "A3DUtils.h"
#include "XmlParser.h"
#include <fstream>
#include <algorithm>

using namespace std;
using namespace assembly3d;
using namespace assembly3d::utils;
using namespace assembly3d::test::mesh;

TesterTool::TesterTool(bool verbose)
    :
    m_verbose(verbose)
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

                std::string tmpAttribName = xml.getAttribute("Attribute", "name", "", i);
                attrib.name = new char[tmpAttribName.length()+1];
                attrib.name[tmpAttribName.length()] = 0;
                memcpy(attrib.name, tmpAttribName.c_str(), tmpAttribName.size());

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
                group.type = new char[typeIndices.length()+1];
                group.type[typeIndices.length()] = 0;
                memcpy(group.type, typeIndices.c_str(), typeIndices.size());

                std::string tmpGrName = xml.getAttribute("Group", "name", "", i);
                group.name = new char[tmpGrName.length()+1];
                group.name[tmpGrName.length()] = 0;
                memcpy(group.name, tmpGrName.c_str(), tmpGrName.size());

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
                         float epsilon,
                         bool ignoreOrderAttributes,
                         bool ignoreOrderGroups)
{
    vector<Attribute> attributesActual;
    vector<Attribute> attributesExpected;
    vector<Group> groupsActual;
    vector<Group> groupsExpected;

    loadMesh(actual, actualBinary, attributesActual, groupsActual);
    loadMesh(expected, expectedBinary, attributesExpected, groupsExpected);

    bool attribsPass = false;
    bool groupsPass = false;
    bool noAttribsCompared = true;
    bool noGroupsCompared = true;

    // compare attributes
    if(attributesActual.size() == attributesExpected.size())
    {
        for(unsigned int i = 0; i < attributesExpected.size(); ++i)
        {
            if(attributeIgnored(attributesExpected[i].name) == false)
            {
                noAttribsCompared = false;
                const Attribute* attribA;
                const Attribute* attribE;
                if(ignoreOrderAttributes)
                {
                    int idx = getAttributeIndexWithName(attributesExpected[i].name,
                                                        attributesActual);
                    if(idx > -1 && idx < (int)attributesExpected.size())
                    {
                        attribA = &attributesActual[idx];
                    }
                    else
                    {
                        attribsPass = false;
                        break;
                    }
                }
                else
                {
                    attribA = &attributesActual[i];
                }
                attribE = &attributesExpected[i];

                if(attribA->count == attribE->count)
                {
                    if(m_verbose)
                    {
                        cout << "Comparing attribute '";
                        cout << attribE->name;
                        cout << "': ";
                    }
                    if(compare(attribE->count,
                               attribA->values,
                               attribE->values,
                               epsilon) == 0)
                    {
                        if(m_verbose)
                            cout << "passed!" << endl;
                        attribsPass = true;
                    }
                    else
                    {
                        if(m_verbose)
                            cout << "failed!" << endl;
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
    }

    // compare groups
    if(groupsActual.size() == groupsExpected.size())
    {
        for(unsigned int i = 0; i < groupsExpected.size(); ++i)
        {
            if(groupIgnored(groupsExpected[i].name) == false)
            {
                noGroupsCompared = false;
                const Group* groupA;
                const Group* groupE;
                if(ignoreOrderGroups)
                {
                    int idx = getGroupIndexWithName(groupsExpected[i].name,
                                                    groupsActual);
                    if(idx > -1 && idx < (int)groupsExpected.size())
                    {
                        groupA = &groupsActual[idx];
                    }
                    else
                    {
                        groupsPass = false;
                        break;
                    }
                }
                else
                {
                    groupA = &groupsActual[i];
                }
                groupE = &groupsExpected[i];

                if(groupA->numBytes == groupE->numBytes)
                {
                    if(m_verbose)
                    {
                        cout << "Comparing group '";
                        cout << groupE->name;
                        cout << "': ";
                    }
                    if(compare(groupE->numBytes,
                               groupA->bytes,
                               groupE->bytes) == 0)
                    {
                        if(m_verbose)
                            cout << "passed!" << endl;
                        groupsPass = true;
                    }
                    else
                    {
                        if(m_verbose)
                            cout << "failed!" << endl;
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
    }

    // print result
    if(noAttribsCompared)
    {
        cout << "No attributes compared!" << endl;
    }
    else
    {
        if(attribsPass)
        {
            cout << "Attributes passed!" << endl;
        }
        else
        {
            cout << "Attributes failed!" << endl;
        }

    }
    if(noGroupsCompared)
    {
        cout << "No groups compared!" << endl;
    }
    else
    {
        if(groupsPass)
        {
            cout << "Groups passed!" << endl;
        }
        else
        {
            cout << "Groups failed!" << endl;
        }
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
        SAFE_DELETE_ARRAY(attribs[i].name)
        SAFE_DELETE_ARRAY(attribs[i].values)
    }
    for(unsigned int i = 0; i < groups.size(); ++i)
    {
        SAFE_DELETE_ARRAY(groups[i].type)
        SAFE_DELETE_ARRAY(groups[i].name)
        SAFE_DELETE_ARRAY(groups[i].bytes)
    }
}

int TesterTool::compare(int n, float* array_a, float* array_e, float epsilon)
{
    for(int i = 0; i < n; ++i)
    {
        if(array_a[i] < array_e[i] - epsilon ||
           array_a[i] > array_e[i] + epsilon )
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

int TesterTool::getAttributeIndexWithName(const char *attrName,
                                          std::vector<Attribute>& attribs)
{
    for(unsigned int i = 0; i < attribs.size(); ++i)
    {
        if(string(attribs[i].name).compare(attrName) == 0)
            return i;
    }
    return -1;
}

int TesterTool::getGroupIndexWithName(const char *groupName,
                                      std::vector<Group> &groups)
{
    for(unsigned int i = 0; i < groups.size(); ++i)
    {
        if(string(groups[i].name).compare(groupName) == 0)
            return i;
    }
    return -1;
}

void TesterTool::setIgnoreListAttributes(std::vector<std::string> ignoreListAttributes)
{
    m_ignoreListAttributes.clear();
    m_ignoreListAttributes = ignoreListAttributes;
}

void TesterTool::setIgnoreListGroups(std::vector<std::string> ignoreListGroups)
{
    m_ignoreListGroups.clear();
    m_ignoreListGroups = ignoreListGroups;
}

bool TesterTool::attributeIgnored(const char *attribName)
{
    vector<string>::iterator result;
    result = find(m_ignoreListAttributes.begin(),
                  m_ignoreListAttributes.end(),
                  attribName);
    if(result == m_ignoreListAttributes.end())
        return false;
    else
        return true;
}

bool TesterTool::groupIgnored(const char *groupName)
{
    vector<string>::iterator result;
    result = find(m_ignoreListGroups.begin(),
                  m_ignoreListGroups.end(),
                  groupName);
    if(result == m_ignoreListGroups.end())
        return false;
    else
        return true;
}
