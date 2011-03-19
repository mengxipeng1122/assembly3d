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

#include "PrimGen.h"

using namespace WizUtils;


int main (int argc, char* argv[])
{
    try {
        TCLAP::CmdLine cmd("MeshPrim - Utility for generating Assembly3D primitives.",
                           '=',
                           MeshPrim::ProjectInfo::versionString);

        // -------------------------------------------------------------------
        TCLAP::ValueArg<std::string> outputFileArg("o", "output-file", "Output file", true, "Prim.mesh.xml", "output-path");

        TCLAP::ValueArg<std::string> planeArg("p", "plane", "Generates plane geometry.", false, "", "plane-args");
        TCLAP::ValueArg<std::string> cubeArg("c", "cube", "Generates cube geometry.", false, "", "cube-args");
        TCLAP::ValueArg<std::string> sphereArg("s", "sphere", "Generates sphere geometry.", false, "", "sphere-args");
        TCLAP::ValueArg<std::string> torusArg("t", "torus", "Generates torus geometry.", false, "", "torus-args");
        // -------------------------------------------------------------------

        std::vector<TCLAP::Arg*> xorlist;
        xorlist.push_back(&planeArg);
        xorlist.push_back(&cubeArg);
        xorlist.push_back(&sphereArg);
        xorlist.push_back(&torusArg);
        cmd.xorAdd(xorlist);

        cmd.add(outputFileArg);

        cmd.parse( argc, argv );

        // -------------------------------------------------------------------

        std::string outputFile;
        std::string outputBinaryFile;

        outputFile = outputFileArg.getValue();
        outputBinaryFile = FileUtils::getBinaryFileName(outputFile.c_str(), ".xml", ".dat");

        PrimGen primGen;

        if(planeArg.isSet())
        {
            primGen.createPlane();
        }
        else if(cubeArg.isSet())
        {
            primGen.createCube();
        }
        else if(sphereArg.isSet())
        {
            primGen.createSphere();
        }
        else if(torusArg.isSet())
        {
            primGen.createTorus();
        }
        else
        {
            return 1;
        }
        primGen.saveFile(outputFile.c_str(), outputBinaryFile.c_str());

    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }

    return 0;
}
