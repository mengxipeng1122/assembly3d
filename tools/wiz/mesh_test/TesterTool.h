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

#ifndef TESTERTOOL_H
#define TESTERTOOL_H

#include "MeshTestIncludes.h"
#include <string>
#include <vector>

namespace assembly3d
{
    namespace test
    {
        namespace mesh
        {
            class TesterTool
            {
            public:
                TesterTool();

                void setIgnoreListAttributes(std::vector<std::string> ignoreListAttributes);
                void setIgnoreListGroups(std::vector<std::string> ignoreListGroups);

                void compare(const char* actual,
                             const char* actualBinary,
                             const char* expected,
                             const char* expectedBinary,
                             float epsilon,
                             bool ignoreOrderAttributes,
                             bool ignoreOrderGroups);

            private:

                void clear(std::vector<Attribute>& attribs,
                           std::vector<Group>& groups);

                int compare(int n, float* array_a, float* array_e, float epsilon);
                int compare(int n, unsigned char* array_a, unsigned char* array_e);

                void loadMesh(const std::string& xmlFile,
                              const std::string& binaryFile,
                              std::vector<Attribute>& attribs,
                              std::vector<Group>& m_groupsActual);

                int getAttributeIndexWithName(const char* attrName,
                                              std::vector<Attribute>& attribs);
                int getGroupIndexWithName(const char* groupName,
                                          std::vector<Group> &groups);

                bool attributeIgnored(const char* attribName);
                bool groupIgnored(const char* groupName);

                std::vector<std::string> m_ignoreListAttributes;
                std::vector<std::string> m_ignoreListGroups;

            };
        }
    }
}

#endif // TESTERTOOL_H
