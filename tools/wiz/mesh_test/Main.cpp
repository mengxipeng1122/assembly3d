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

#include "MeshTestIncludes.h"
#include <tclap/CmdLine.h>
#include "TesterTool.h"
#include "A3DUtils.h"

using namespace assembly3d;
using namespace assembly3d::utils;
using namespace assembly3d::test::mesh;


int main (int argc, char* argv[])
{
    try {
        TCLAP::CmdLine cmd("MeshTester - Utility for comparing Assembly3D mesh files.",
                           '=',
                           ProjectInfo::versionString);

        // -------------------------------------------------------------------

        TCLAP::ValueArg<std::string> actualArg("a",
                                               "actual",
                                               "Actual file.",
                                               true,
                                               "",
                                               "file-path");

        TCLAP::ValueArg<std::string> actualBinaryArg("", "actual-binary",
                                                     "Actual input binary file (default: same as actual file but with extension '.dat').",
                                                     false, "",
                                                     "file-path");
        TCLAP::ValueArg<std::string> expectedArg("e",
                                                 "expected",
                                                 "Expected file.",
                                                 true,
                                                 "",
                                                 "file-path");

        TCLAP::ValueArg<std::string> expectedBinaryArg("",
                                                       "expected-binary",
                                                       "Expected binary file (default: same as expected file but with extension '.dat').",
                                                       false, "",
                                                       "file-path");
        TCLAP::ValueArg<float> epsilonArg("",
                                          "epsilon",
                                          "Compares attribute values with a deviation epsilon (default: 0.0).",
                                          false,
                                          0.0f,
                                          "val");

        TCLAP::SwitchArg ignoreOrderAttributesArg("", "ignore-order-attributes", "Ignores attribute order.", false);
        TCLAP::SwitchArg ignoreOrderGroupsArg("", "ignore-order-groups", "Ignores group order.", false);

        TCLAP::ValueArg<std::string> excludeAttributesArg("", "exclude-attributes", "List with attributes to exclude.",
                                                          false, "", "name-list");
        TCLAP::ValueArg<std::string> excludeGroupsArg("", "exclude-groups", "List with groups to exclude",
                                                      false, "", "name-list");
        TCLAP::SwitchArg quiteArg("q", "quite", "No output verbosity.", false);

        cmd.add(quiteArg);
        cmd.add(excludeAttributesArg);
        cmd.add(excludeGroupsArg);
        cmd.add(ignoreOrderAttributesArg);
        cmd.add(ignoreOrderGroupsArg);
        cmd.add(epsilonArg);
        cmd.add(expectedBinaryArg);
        cmd.add(expectedArg);
        cmd.add(actualBinaryArg);
        cmd.add(actualArg);

        // Parse the argv array.
        cmd.parse( argc, argv );

        // -------------------------------------------------------------------

        std::string actualFile = actualArg.getValue();
        std::string expectedFile = expectedArg.getValue();

        if(FileUtils::checkIfFileExists(actualFile.c_str()) == false)
        {
            std::cerr << "Error: Actual file does not exist!" << std::endl;
            return 1;
        }
        if(FileUtils::checkIfFileExists(expectedFile.c_str()) == false)
        {
            std::cerr << "Error: Expected file does not exist!" << std::endl;
            return 1;
        }

        std::string actualBinaryFile;
        std::string expectedBinaryFile;

        if(actualBinaryArg.isSet())
        {
            actualBinaryFile = actualBinaryArg.getValue();
        }
        else
        {
            actualBinaryFile = FileUtils::getBinaryFileName(actualFile.c_str(), ".xml", ".dat");
        }

        if(expectedBinaryArg.isSet())
        {
            expectedBinaryFile = expectedBinaryArg.getValue();
        }
        else
        {
            expectedBinaryFile = FileUtils::getBinaryFileName(expectedFile.c_str(), ".xml", ".dat");
        }

        if(FileUtils::checkIfFileExists(actualBinaryFile.c_str()) == false)
        {
            std::cerr << "Error: Actual binary file does not exist!" << std::endl;
            return 1;
        }
        if(FileUtils::checkIfFileExists(expectedBinaryFile.c_str()) == false)
        {
            std::cerr << "Error: Expected binary file does not exist!" << std::endl;
            return 1;
        }

        float eps = epsilonArg.getValue();

        bool ignoreOrderAttributes = ignoreOrderAttributesArg.getValue();
        bool ignoreOrderGroups = ignoreOrderGroupsArg.getValue();

        bool verboseOutput = !quiteArg.getValue();

        TesterTool tester(verboseOutput);

        if(excludeAttributesArg.isSet())
        {
            std::string argsStr = excludeAttributesArg.getValue();
            std::vector<std::string> values;

            StringUtils::getStrValuesFromCmdString(argsStr, values);

            //transform values to uuper case.
            StringUtils::transformStringValuesToUpperCase(values);

            tester.setIgnoreListAttributes(values);

        }
        if(excludeGroupsArg.isSet())
        {
            std::string argsStr = excludeGroupsArg.getValue();
            std::vector<std::string> values;

            StringUtils::getStrValuesFromCmdString(argsStr, values);

            tester.setIgnoreListGroups(values);
        }

        tester.compare(actualFile.c_str(),
                       actualBinaryFile.c_str(),
                       expectedFile.c_str(),
                       expectedBinaryFile.c_str(),
                       eps,
                       ignoreOrderAttributes,
                       ignoreOrderGroups);

    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }

    return 0;
}
