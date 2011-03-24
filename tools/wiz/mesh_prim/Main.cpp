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
#include "A3DUtils.h"
#include "MeshIO.h"

#include "PrimGen.h"

using namespace assembly3d;
using namespace assembly3d::utils;
using namespace assembly3d::prim::mesh;
using namespace TCLAP;

int main (int argc, char* argv[])
{
    try {

        CmdLine cmd("MeshPrim - Utility for generating Assembly3D primitives.",
                           '=',
                           ProjectInfo::versionString);

        // -------------------------------------------------------------------

        // TODO: set default values

        ValueArg<std::string> outputNameArg("o", "output-file-name", "Output file name.",
                                            true, "Prim.mesh.xml", "output-name");
        ValueArg<std::string> outputDirArg("d", "output-dir", "Output directory (default=.).",
                                           false, ".", "path");

        ValueArg<std::string> planeArg("", "plane", "Generates plane geometry.",
                                       false, "", "half-extend");
        ValueArg<std::string> cubeArg("", "cube", "Generates cube geometry.",
                                      false, "", "half-extend");
        ValueArg<std::string> sphereArg("", "sphere", "Generates sphere geometry.",
                                        false, "", "radius/slices/stacks");
        ValueArg<std::string> torusArg("", "torus", "Generates torus geometry.",
                                       false, "", "inner/outer/slices/stacks");

        ValueArg<std::string> diskArg("", "disk", "Generates disk primitive.",
                                      false, "", "inner/outer/slices/stacks");
        ValueArg<std::string> cylinderArg("", "cylinder", "Generates cylinder primitive.",
                                          false, "", "base/top/height/slices/stacks");
        ValueArg<std::string> trapezoidArg("", "trapezoid", "Generates trapezoid primitive.",
                                           false, "", "base/top/height/slices/stacks");
        ValueArg<std::string> rectangleArg("", "rectangle", "Generates rectangle primitive.",
                                           false, "", "extX/extY/slices/stacks");
        ValueArg<std::string> partialDiskArg("", "partial-disk", "Generates partial disk primitive.",
                                             false, "", "inner/outer/slices/stacks/start/sweep");
        ValueArg<std::string> partialCylinderArg("", "partial-cylinder", "Generates partial cylinder primitive.",
                                                 false, "", "base/top/height/slices/stacks/start/sweep");

        ValueArg<std::string> generateAttribsArg("", "generate-attributes", "Specifies which attribute to generate.",
                                                 false, "positions/normals/texcoords", "positions/normals/texcoords");

        // -------------------------------------------------------------------

        std::vector<Arg*> xorlist;
        xorlist.push_back(&planeArg);
        xorlist.push_back(&cubeArg);
        xorlist.push_back(&sphereArg);
        xorlist.push_back(&torusArg);
        xorlist.push_back(&diskArg);
        xorlist.push_back(&cylinderArg);
        xorlist.push_back(&trapezoidArg);
        xorlist.push_back(&rectangleArg);
        xorlist.push_back(&partialDiskArg);
        xorlist.push_back(&partialCylinderArg);
        cmd.xorAdd(xorlist);

        cmd.add(outputDirArg);
        cmd.add(generateAttribsArg);
        cmd.add(outputNameArg);

        cmd.parse( argc, argv );

        // -------------------------------------------------------------------

        std::string outputDir;
        std::string outputFile;
        std::string outputBinaryFile;

        outputDir = outputDirArg.getValue();

        char sep = '/';
#ifdef TARGET_WIN32
        sep = '\\';
#endif
        outputFile = outputDir + sep + outputNameArg.getValue();

        outputBinaryFile = FileUtils::getBinaryFileName(outputFile.c_str(), ".xml", ".dat");

        bool genPos = true;
        bool genNorm = true;
        bool genTex = true;
//        bool genTan = false;
//        bool genBitan = false;

        if(generateAttribsArg.isSet())
        {
            std::string attribsArgs = generateAttribsArg.getValue();
            std::vector<std::string> genValues;
            StringUtils::getStrValuesFromCmdString(attribsArgs, genValues);

            genPos = false;
            genNorm = false;
            genTex = false;

            std::vector<std::string>::iterator it;
            for(it = genValues.begin(); it != genValues.end(); ++it)
            {
                if(it->compare("positions") == 0)
                {
                    genPos = true;
                }
                else if(it->compare("normals") == 0)
                {
                    genNorm = true;
                }
                else if(it->compare("texcoords") == 0)
                {
                    genTex = true;
                }
            }
            if(!genPos && !genNorm && !genTex)
            {
                std::cerr << "There are no attributes to generate" << std::endl;
                return 1;
            }
        }

        // -------------------------------------------------------------------

        Mesh mesh;
        PrimGen primGen(genPos, genNorm, genTex);

        // -------------------------------------------------------------------

        if(planeArg.isSet())
        {
            float val = 0.0f;
            StringUtils::getValueFromCmdString(planeArg.getValue(), val);
            std::vector<float> values;
            values.push_back(val);

            primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_PLANE, values);
        }
        else if(cubeArg.isSet())
        {
            float val = 0.0f;
            StringUtils::getValueFromCmdString(cubeArg.getValue(), val);
            std::vector<float> values;
            values.push_back(val);

            primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_CUBE, values);
        }
        else if(sphereArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(sphereArg.getValue(), values);

            if(values.size() == 3)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_SPHERE, values);
            }
        }
        else if(torusArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(torusArg.getValue(), values);

            if(values.size() == 4)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_TORUS, values);
            }
        }
        else if(diskArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(diskArg.getValue(), values);

            if(values.size() == 4)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_DISK, values);
            }
        }
        else if(cylinderArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(cylinderArg.getValue(), values);

            if(values.size() == 5)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_CYLINDER, values);
            }
        }
        else if(rectangleArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(rectangleArg.getValue(), values);

            if(values.size() == 4)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_RECTANGLE, values);
            }
        }
        else if(trapezoidArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(trapezoidArg.getValue(), values);

            if(values.size() == 5)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_TRAPEZOID, values);
            }
        }
        else if(partialDiskArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(partialDiskArg.getValue(), values);

            if(values.size() == 6)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_PARTIAL_DISK, values);
            }
        }
        else if(partialCylinderArg.isSet())
        {
            std::vector<float> values;
            StringUtils::getValuesFromCmdString(partialCylinderArg.getValue(), values);

            if(values.size() == 7)
            {
                primGen.createMesh(&mesh, PrimGen::PRIM_TYPE_PARTIAL_CYLINDER, values);
            }
        }
        else
            // Should never get here because TCLAP will note that one of the
            // required args above has not been set.
            throw("Very bad things...");

        // -------------------------------------------------------------------

        MeshIO::saveFile(&mesh, outputFile.c_str(), outputBinaryFile.c_str());

        // -------------------------------------------------------------------

    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }

    return 0;
}
