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
#include "A3DUtils.h"
#include <tclap/CmdLine.h>
#include "Mesh.h"
#include "MeshIO.h"
#include "ToolManager.h"

using namespace assembly3d;
using namespace assembly3d::utils;
using namespace assembly3d::wiz::mesh;

//==============================================================================
int main (int argc, char* argv[])
{
    std::string inputfile;
    std::string outputfile;
    bool verbose = true;
    //bool debug = false;
    bool modelChanged = false;
    

    try {  
	
    TCLAP::CmdLine cmd("MeshWiz - Utility for manipulating Assembly3D mesh files.",
                       '=',
                       ProjectInfo::versionString);

    //---------------------------------------------------------------------------------------------------------
    // Input / Output
    //---------------------------------------------------------------------------------------------------------
    TCLAP::UnlabeledValueArg<std::string> inputArg("source-file",
                                                   "File to manipulate.",
                                                   true,
                                                   "",
                                                   "source-file");

    TCLAP::ValueArg<std::string> outputArg("o",
                                           "output-folder",
                                           "Output folder.",
                                           false,
                                           "processed",
                                           "output-folder");

    TCLAP::ValueArg<std::string> binaryOutputArg("b",
                                                 "binary-file",
                                                 "Name of binary file.",
                                                 false,
                                                 "",
                                                 "binary-file");
    
    //---------------------------------------------------------------------------------------------------------
    // Transform
    //---------------------------------------------------------------------------------------------------------
    TCLAP::ValueArg<std::string> translateArg("t",
                                              "translate",
                                              "Translates the mesh by this vector.",
                                              false,
                                              "",
                                              "x/y/z");

    TCLAP::ValueArg<std::string> rotateArg("r",
                                           "rotate",
                                           "Rotates the mesh <angle> degrees on the axis x/y/z.",
                                           false,
                                           "",
                                           "angle/x/y/z");

    TCLAP::ValueArg<std::string> scaleArg("s",
                                          "scale",
                                          "Scales the mesh by this scale vector or scales by one scale factor s.",
                                          false,
                                          "",
                                          "x/y/z|s");

    TCLAP::ValueArg<std::string> resizeArg("",
                                           "resize",
                                           "Scales the mesh so, that its size is x/y/z or give axis and new value r.",
                                           false,
                                           "",
                                           "x/y/z|axis/r");

    TCLAP::ValueArg<std::string> centerArg("",
                                           "center",
                                           "Centers mesh to given axis (i.e. 1/1/1 to put center to 0/0/0).",
                                           false,
                                           "",
                                           "1/1/1");

    TCLAP::SwitchArg centerAllArg("",
                                  "center-all",
                                  "Centers mesh to all axis.",
                                  false);
    
    //---------------------------------------------------------------------------------------------------------
    // Conversion
    //---------------------------------------------------------------------------------------------------------
    std::vector<std::string> conversionIndexTypeToAllowed;
    conversionIndexTypeToAllowed.push_back("int");
    conversionIndexTypeToAllowed.push_back("short");
    conversionIndexTypeToAllowed.push_back("byte");
    TCLAP::ValuesConstraint<std::string> conversionIndexTypeToAllowedVals( conversionIndexTypeToAllowed );
    TCLAP::ValueArg<std::string> convertIndexTypeToArg("",
                                                       "convert-index-type-to",
                                                       "Converts index type between UNSIGNED_INT , UNSIGNED_SHORT and UNSIGNED_BYTE.",
                                                       false,
                                                       "",
                                                       &conversionIndexTypeToAllowedVals);

    //---------------------------------------------------------------------------------------------------------
    // Normals / Tangents
    //---------------------------------------------------------------------------------------------------------
    std::vector<std::string> normalsTangentsAllowed;
    normalsTangentsAllowed.push_back("generate");
    normalsTangentsAllowed.push_back("remove");
    TCLAP::ValuesConstraint<std::string> normalsTangentsAllowedVals( normalsTangentsAllowed );
    TCLAP::ValueArg<std::string> normalsArg("",
                                            "normals",
                                            "Generates or removes mesh normals.",
                                            false,
                                            "",
                                            &normalsTangentsAllowedVals);

    TCLAP::ValueArg<std::string> tangentsArg("",
                                             "tangents",
                                             "Generates or removes mesh tangents.",
                                             false,
                                             "",
                                             &normalsTangentsAllowedVals);

    TCLAP::ValueArg<std::string> bitangentsArg("",
                                               "bitangents",
                                               "Generates or removes mesh bitangents.",
                                               false,
                                               "",
                                               &normalsTangentsAllowedVals);
    
    //---------------------------------------------------------------------------------------------------------
    // Optimization
    //---------------------------------------------------------------------------------------------------------
    TCLAP::SwitchArg optimizeVerticesArg("",
                                         "optimize-vertices",
                                         "Optimizes vertices order for GPU cache.",
                                         false);

    TCLAP::SwitchArg optimizeIndicesArg("",
                                        "optimize-indices",
                                        "Optimizes indices order for GPU cache.",
                                        false);

    TCLAP::SwitchArg stitchArg("",
                               "stitch",
                               "Removes duplicate vertices.",
                               false);

    std::vector<std::string> stitchAllowed;
    stitchAllowed.push_back("position/epsilon");
    stitchAllowed.push_back("normal/epsilon");
    stitchAllowed.push_back("texture/epsilon");
    stitchAllowed.push_back("tangent/epsilon");
    stitchAllowed.push_back("bitangent/epsilon");
    TCLAP::ValuesConstraint<std::string> stitchAllowedVals( stitchAllowed );
    TCLAP::ValueArg<std::string> stitchEpsArg("",
                                              "stitch-eps",
                                              "Removes duplicate vertices. Compares all attributes but one given attribute with a possible deviation epsilon.",
                                              false,
                                              "",
                                              &stitchAllowedVals);

    //---------------------------------------------------------------------------------------------------------
    // Rename
    //---------------------------------------------------------------------------------------------------------
    TCLAP::ValueArg<std::string> renameIdsArg("",
                                              "rename-ids",
                                              "Renames group ids.",
                                              false,
                                              "",
                                              "new-name");
    
    //---------------------------------------------------------------------------------------------------------
    // Other mesh related stuff
    //---------------------------------------------------------------------------------------------------------
////    std::vector<std::string> flipAllowed;
////    flipAllowed.push_back("cw");
////    flipAllowed.push_back("ccw");
////    TCLAP::ValuesConstraint<std::string> flipAllowedVals( flipAllowed );
//    TCLAP::ValueArg<std::string> flipArg("",
//                                         "flip",
//                                         "Flips front-face clockwise or counterclockwise.",
//                                         false,
//                                         "",
//                                         &flipAllowedVals);

    TCLAP::SwitchArg flipArg("", "flip-front-face", "Flips fron-faces.");

    TCLAP::SwitchArg checkWithNormalsArg("",
                                         "check-with-normals",
                                         "Checks with normals (if present).",
                                         false);

    TCLAP::SwitchArg validateAndChangeArg("",
                                          "validate-and-change",
                                          "Validates and changes mesh.",
                                          false);

    //---------------------------------------------------------------------------------------------------------
    // Other
    //---------------------------------------------------------------------------------------------------------
    TCLAP::SwitchArg quiteArg("q", "quite", "No verbose output.", false);
    
    TCLAP::SwitchArg infoArg("i", "info", "Prints the mesh info.", false);

    TCLAP::SwitchArg dumpArg("", "dump-txt", "Dumps the mesh to a text file.", false);
    //---------------------------------------------------------------------------------------------------------
    // Adding args to cmd
    //---------------------------------------------------------------------------------------------------------
    cmd.add(infoArg);
    cmd.add(dumpArg);
    cmd.add(quiteArg);
    cmd.add(binaryOutputArg);
    cmd.add(inputArg);
    cmd.add(outputArg);
    cmd.add(flipArg);
//    cmd.add(checkWithNormalsArg);
//    cmd.add(validateAndChangeArg);
//    cmd.add(optimizeIndicesArg);
//    cmd.add(optimizeVerticesArg);
    cmd.add(stitchEpsArg);
    cmd.add(stitchArg);
    cmd.add(bitangentsArg);
    cmd.add(tangentsArg);
    cmd.add(normalsArg);
    cmd.add(convertIndexTypeToArg);
    cmd.add(centerAllArg);
    cmd.add(centerArg);
    cmd.add(resizeArg);
    cmd.add(scaleArg);
    cmd.add(rotateArg);
    cmd.add(translateArg);
    
	// Parse the argv array.
	cmd.parse( argc, argv );

    //---------------------------------------------------------------------------------------------------------

//    std::cout << std::endl;
    verbose = !quiteArg.getValue();
    if(verbose)
    {
        std::cout << cmd.getMessage() << std::endl;
        std::cout << std::endl;
    }

    //---------------------------------------------------------------------------------------------------------

    std::string sep = "/";
#ifdef TARGET_WIN32
    sep = "\\";
#endif

    inputfile = inputArg.getValue();
    if(FileUtils::checkIfFileExists(inputfile.c_str()) == false)
    {
        std::cerr << "Error: Input source '" << inputfile << "', does not exist!" << std::endl;
        return 1;
    }
    if(infoArg.isSet() == false)
    {
        std::string outputdir = outputArg.getValue();
        if(FileUtils::checkIfDirectoryExists(outputdir.c_str()) == false)
        {
            FileUtils::createDirectory(outputdir.c_str());
        }
        std::string infilename = FileUtils::getFileName(inputfile);
        outputfile = outputdir+sep+infilename;

    }
    
    //---------------------------------------------------------------------------------------------------------

    std::string binaryInFileName;
    std::string binaryOutFileName;

    if(binaryOutputArg.isSet())
    {
        binaryInFileName = binaryOutputArg.getValue();
        binaryOutFileName = outputArg.getValue()+sep+binaryOutputArg.getValue();
    }
    else
    {
        size_t posdot = inputfile.find(".xml");
        binaryInFileName = inputfile.substr(0, posdot);
        binaryInFileName.append(".dat");

        std::string infilename = FileUtils::getFileName(inputfile);
        size_t pos = infilename.find(".xml");
        std::string tmp = infilename.substr(0, pos);
        
        
        binaryOutFileName = outputArg.getValue();
        binaryOutFileName.append(sep+tmp);
        binaryOutFileName.append(".dat");
    }

    //---------------------------------------------------------------------------------------------------------
    Mesh mesh;
    if(MeshIO::load(&mesh, inputfile.c_str(), binaryInFileName.c_str()) == false)
    {
        std::cerr << "Error: Loading '" << inputfile << "', failed!" << std::endl;
        return 1;
    }
    if(FileUtils::checkIfFileExists(binaryInFileName.c_str()) == false && mesh.getMeshFormat().isBinary)
    {
        std::cerr << "Error: Binary file not found! " << std::endl;
        return 1;
    }


    ToolManager toolMgr(&mesh, verbose);
    
    //---------------------------------------------------------------------------------------------------------
    if(verbose)
    {
        std::cout << "Input file: " << inputfile << std::endl;
        if(mesh.getMeshFormat().isBinary)
            std::cout << "Binary file: " << binaryInFileName << std::endl;

        std::cout << "Output path: " << outputArg.getValue() << std::endl;

        std::cout << std::endl;
    }

    //---------------------------------------------------------------------------------------------------------
    if(infoArg.getValue())
    {
        std::cout << mesh << std::endl;
        return 0;
    }
    
    //---------------------------------------------------------------------------------------------------------
    if(convertIndexTypeToArg.isSet())
    {
        if(toolMgr.convertIndexType(convertIndexTypeToArg.getValue().c_str()) == false)
        {
            std::cerr << "Error: To many vertices. It is not possible to have indices of the type '";
            std::cerr << convertIndexTypeToArg.getValue() << "'" << std::endl;
            return 1;
        }
        modelChanged = true;
    }
    
    //---------------------------------------------------------------------------------------------------------
    if(normalsArg.isSet())
    {
        if(normalsArg.getValue().compare("generate")==0)
        {
            toolMgr.generateNormals();
            
        }
        else if(normalsArg.getValue().compare("remove")==0)
        {
            toolMgr.removeNormals();
        }
        modelChanged = true;
    }
    if(tangentsArg.isSet())
    {
        if(tangentsArg.getValue().compare("generate")==0)
        {
            toolMgr.generateTangents();
        }
        else if(tangentsArg.getValue().compare("remove")==0)
        {
            toolMgr.removeTangents();
        }
        modelChanged = true;
    }
    if(bitangentsArg.isSet())
    {
        if(bitangentsArg.getValue().compare("generate")==0)
        {
            toolMgr.generateBitangents();
        }
        else if(bitangentsArg.getValue().compare("remove")==0)
        {
            toolMgr.removeBitangents();
        }
        modelChanged = true;
    }
    
    //---------------------------------------------------------------------------------------------------------
    
    if(translateArg.isSet())
    {
        std::string args = translateArg.getValue();
        float x, y, z;
        x = y = z = 0.0f;
        std::vector<float> values;
        StringUtils::getValuesFromCmdString(args, values);

        if(values.size() == 3)
        {
            x = values[0];
            y = values[1];
            z = values[2];
            toolMgr.translate(x, y, z);
            modelChanged = true;
        }
    }
    if(rotateArg.isSet())
    {
        std::string args = rotateArg.getValue();
        float angle, x, y, z;
        angle = x = y = z = 0.0f;

        std::vector<float> values;        
        StringUtils::getValuesFromCmdString(args, values);
        if(values.size() == 4)
        {
            angle = values[0];
            x = values[1];
            y = values[2];
            z = values[3];
            toolMgr.rotate(angle, x, y, z);
            modelChanged = true;
        }
    }
    if(scaleArg.isSet())
    {
        std::string args = scaleArg.getValue();
        
        float x, y, z;
        x = y = z = 0.0f;
        std::vector<float> values;
        
        StringUtils::getValuesFromCmdString(args, values);
        
        if(values.size() == 3)
        {
            x = values[0];
            y = values[1];
            z = values[2];
            toolMgr.scale(x, y, z);
            modelChanged = true;            
        }
        else if(values.size() == 1)
        {
            x = values[0];
            toolMgr.scale(x, x, x);
            modelChanged = true;
        }
    }
    if(resizeArg.isSet())
    {
        std::string args = resizeArg.getValue();
        float x, y, z;
        x = y = z = 0.0f;
        std::vector<float> values;
        
        int numSlashes = StringUtils::findOccurensesOf(args, sep);
        if(numSlashes == 2)
        {
            StringUtils::getValuesFromCmdString(args, values);
            if(values.size() == 3)
            {
                x = values[0];
                y = values[1];
                z = values[2];
//                toolMgr.scale(x, y, z);
                toolMgr.resize(x, y, z);
                modelChanged = true;
            }
        }
        else if(numSlashes == 1)
        {
            std::string cmdStr = args;
            int pos = cmdStr.find(sep);
            std::string axis = cmdStr.substr(0, pos);
            cmdStr = cmdStr.erase(0, pos+1);

            StringUtils::getValuesFromCmdString(cmdStr, values);
            if(values.size() == 1)
            {
                x = values[0];
                toolMgr.resize(axis.c_str(), x);
                modelChanged = true;
            }
        }
    }
    
    //---------------------------------------------------------------------------------------------------------

    if(stitchArg.isSet())
    {
        toolMgr.stitch();
        modelChanged = true;
    }
    if(stitchEpsArg.isSet())
    {
        std::string args = stitchEpsArg.getValue();
        float eps = 0.0f;
        std::vector<float> values;

        int numSlashes = StringUtils::findOccurensesOf(args, sep);
        if(numSlashes == 1)
        {
            std::string cmdStr = args;
            int pos = cmdStr.find(sep);
            std::string attributeName = cmdStr.substr(0, pos);
            cmdStr = cmdStr.erase(0, pos+1);

            StringUtils::getValuesFromCmdString(cmdStr, values);
            if(values.size() == 1)
            {
                eps = values[0];
                toolMgr.stitchEps(attributeName.c_str(), eps);
                modelChanged = true;
            }
        }
    }
    
    //---------------------------------------------------------------------------------------------------------

    if(centerArg.isSet())
    {
        std::string args = centerArg.getValue();
        int axisX, axisY, axisZ;
        axisX = axisY = axisZ = 0;
        std::vector<float> values;

        StringUtils::getValuesFromCmdString(args, values);
        if(values.size() == 3)
        {
            axisX = (int)values[0];
            axisY = (int)values[1];
            axisZ = (int)values[2];
            toolMgr.center(axisX, axisY, axisZ);
        
            modelChanged = true;
        }
    }
    else if(centerAllArg.isSet())
    {
        toolMgr.center(1, 1, 1);
        modelChanged = true;
    }

    //---------------------------------------------------------------------------------------------------------
    if(flipArg.isSet())
    {
        toolMgr.flip();
        modelChanged = true;
    }

    //---------------------------------------------------------------------------------------------------------
    if(dumpArg.isSet())
    {
        std::string debugOutputFile;
        size_t posdot = outputfile.find(".xml");
        debugOutputFile = outputfile.substr(0, posdot);
        debugOutputFile.append(".txt");

        MeshIO::dumpTxt(&mesh, debugOutputFile.c_str());
        std::cout << "Dump mesh to text file" << std::endl;
    }

    if(modelChanged)
    {
        MeshIO::saveFile(&mesh, outputfile.c_str(), binaryOutFileName.c_str());
        std::cout << "Done!" << std::endl;
    }
    else
    {
        std::cout << "No modification" << std::endl;
    }
    //---------------------------------------------------------------------------------------------------------
    
	} catch (TCLAP::ArgException &e)  // catch any exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }   
    
    return 0;
}
