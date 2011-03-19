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

#include "MeshPrimIncludes.h"
#include <tclap/CmdLine.h>
#include "WizUtils.h"
#include "MeshIO.h"

#include "PrimGen.h"

using namespace WizUtils;


int main (int argc, char* argv[])
{
    try {
        TCLAP::CmdLine cmd("MeshPrim - Utility for generating Assembly3D primitives.",
                           '=',
                           MeshPrim::ProjectInfo::versionString);

        // -------------------------------------------------------------------
        TCLAP::ValueArg<std::string> outputNameArg("o", "output-file-name", "Output file name",
                                                   true, "Prim.mesh.xml", "output-name");
        TCLAP::ValueArg<std::string> outputDirArg("d", "output-dir", "Output directory (default=./)",
                                                  false, "./", "path");

        TCLAP::ValueArg<std::string> planeArg("p", "plane", "Generates plane geometry.",
                                              false, "", "half-extend");
        TCLAP::ValueArg<std::string> cubeArg("c", "cube", "Generates cube geometry.",
                                             false, "", "half-extend");
        TCLAP::ValueArg<std::string> sphereArg("s", "sphere", "Generates sphere geometry.",
                                               false, "", "radius/slices");
        TCLAP::ValueArg<std::string> torusArg("t", "torus", "Generates torus geometry.",
                                              false, "", "inner-radius/outer-radius/sides/faces");
        // -------------------------------------------------------------------

        std::vector<TCLAP::Arg*> xorlist;
        xorlist.push_back(&planeArg);
        xorlist.push_back(&cubeArg);
        xorlist.push_back(&sphereArg);
        xorlist.push_back(&torusArg);
        cmd.xorAdd(xorlist);

        cmd.add(outputDirArg);
        cmd.add(outputNameArg);

        cmd.parse( argc, argv );

        // -------------------------------------------------------------------

        std::string outputDir;
        std::string outputFile;
        std::string outputBinaryFile;

        outputDir = outputDirArg.getValue();

        outputFile = outputDir.append(outputNameArg.getValue());

        outputBinaryFile = outputDir.append(FileUtils::getBinaryFileName(outputFile.c_str(), ".xml", ".dat"));

        Mesh* mesh = new Mesh();
        PrimGen primGen;

        if(planeArg.isSet())
        {
            std::string args = planeArg.getValue();
            float val = 0.0f;
            StringUtils::getValueFromCmdString(args, val);

            primGen.createMesh(mesh, PrimGen::PRIM_TYPE_PLANE, 1, val);
        }
        else if(cubeArg.isSet())
        {
            std::string args = planeArg.getValue();
            float val = 0.0f;
            StringUtils::getValueFromCmdString(args, val);

            primGen.createMesh(mesh, PrimGen::PRIM_TYPE_CUBE, 1, val);
        }
        else if(sphereArg.isSet())
        {
            std::string args = planeArg.getValue();

            std::vector<float> values;
            StringUtils::getValuesFromCmdString(args, values);

            if(values.size() == 2)
            {
                primGen.createMesh(mesh, PrimGen::PRIM_TYPE_SPHERE, 2,
                                   values[0], values[1]);
            }
        }
        else if(torusArg.isSet())
        {
            std::string args = planeArg.getValue();

            std::vector<float> values;
            StringUtils::getValuesFromCmdString(args, values);

            if(values.size() == 4)
            {
                primGen.createMesh(mesh, PrimGen::PRIM_TYPE_TORUS, 4,
                                   values[0], values[1], values[2], values[3]);
            }
        }
        else
        {
            return 1;
        }
        if(mesh)
            MeshIO::saveFile(mesh, outputFile.c_str(), outputBinaryFile.c_str());


    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }

    return 0;
}
