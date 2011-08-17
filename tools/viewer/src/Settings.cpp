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

#include "Settings.h"
#include <tclap/CmdLine.h>
#include "Utils.h"
#include "Resources.h"

Settings::Settings()
{
}

bool Settings::load(Resources* r, int argc, char *argv[])
{
    try {
       using namespace TCLAP;
       CmdLine cmd("Assembly3D Viewer", '=', "1.0.0");

       UnlabeledValueArg<std::string> metaFileArg("input-file", "Input file.", true, "", "path/to/mesh");
       ValueArg<std::string> dataFileArg("b", "binary-file", "Binary file.", false, "", "path/to/binary");
       ValueArg<float> meshScaleArg("s", "scale", "Scale.", false, 1.0f, "scale");

       cmd.add(meshScaleArg);
       cmd.add(dataFileArg);
       cmd.add(metaFileArg);

       cmd.parse(argc, argv);

       r->meshPath = metaFileArg.getValue();
       if(Utils::checkIfFileExists(r->meshPath.c_str()) == false)
       {
           std::cout << "Mesh file not found!" << std::endl;
           return 1;
       }
       if(dataFileArg.isSet())
       {
           r->dataPath = dataFileArg.getValue();
       }
       else
       {
           size_t pos = r->meshPath.find(".xml");
           r->dataPath = r->meshPath.substr(0, pos);
           r->dataPath.append(".dat");
       }
       if(Utils::checkIfFileExists(r->dataPath.c_str()) == false)
       {
           std::cout << "Mesh file not found!" << std::endl;
           return 1;
       }

       r->scale = meshScaleArg.getValue();

       size_t pos = r->meshPath.find_last_of("/");
       if(pos == std::string::npos)
       {
           r->texPath = "./";
       }
       else
       {
           r->texPath = r->meshPath.substr(0, pos+1);
       }
       return true;
     } catch (TCLAP::ArgException &e)  // catch any exceptions
     { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; return false;}

}
