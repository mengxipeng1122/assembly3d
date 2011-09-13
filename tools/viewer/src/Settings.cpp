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
#include <vector>
#include <libxml/xmlreader.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SceneLoader.h"
#include "Resources.h"

using namespace std;
using namespace TCLAP;

Settings::Settings()
{
}

bool Settings::load(Resources* r, int argc, char *argv[])
{
    try {
        CmdLine cmd("Assembly3D Viewer", '=', "1.0.0");

        ValueArg<string> sceneFileArg("s", "scene-file", "Scene file.", false, "", "path/to/scene");
        ValueArg<string> meshDirArg("d", "mesh-dir", "Mesh directory.", false, "", "path/to/meshes");
        ValueArg<string> metaFileArg("m", "mesh-file", "Mesh file.", false, "", "path/to/mesh");
        ValueArg<string> dataFileArg("b", "binary-file", "Binary file.", false, "", "path/to/binary");
        ValueArg<float> meshScaleArg("", "scale", "Scale.", false, 1.0f, "scale");
        ValueArg<float> sceneScaleArg("", "scene-scale", "Scene scale.", false, 1.0f, "scale");
        ValueArg<string> animArg("a", "anim-file", "Animation file.", false, "", "path/to/animatoion");

        cmd.add(sceneScaleArg);
        cmd.add(meshScaleArg);
        cmd.add(dataFileArg);
        cmd.add(metaFileArg);
        cmd.add(meshDirArg);
        cmd.add(sceneFileArg);
        cmd.add(animArg);

        cmd.parse(argc, argv);

        if(metaFileArg.isSet() == false && sceneFileArg.isSet() == false)
        {
            StdOutput stdOut;
            ArgException ex("No mesh file or scene file provided!");
            stdOut.failure(cmd, ex);
            return false;
        }
        if(sceneFileArg.isSet())
        {
            r->hasScene = true;
            r->scenePath = sceneFileArg.getValue();
            size_t sceneNamePos = r->scenePath.find_last_of("/");
            string sceneName;
            if(sceneNamePos == std::string::npos)
            {
                size_t posext = r->scenePath.find_first_of(".");
                sceneName = r->scenePath.substr(0, posext);
            }
            else
            {
                string tmp = r->scenePath.substr(sceneNamePos+1);
                size_t posext = tmp.find_first_of(".");
                sceneName = tmp.substr(0, posext);
            }
            
            if(meshDirArg.isSet())
            {
                string meshDir = meshDirArg.getValue();
                char lastChar = meshDir.at(meshDir.length()-1);
                if(lastChar != '/')
                    meshDir.append("/");
                r->sceneDir = meshDir;
//                r->useMeshDir = true;
            }
            else
            {
                size_t pos = r->scenePath.find_last_of("/");
                if(pos == std::string::npos)
                {
                    r->sceneDir = "./"+sceneName+"/";
                }
                else
                {
                    r->sceneDir = r->scenePath.substr(0, pos);
                    r->sceneDir.append("/"+sceneName+"/");
                }
                //r->useMeshDir = false;
            }
            
            if(Utils::checkIfFileExists(r->scenePath.c_str()) == false)
            {
                cout << "Scene file not found!" << endl;
                return false;
            }
            
            r->sceneScale = sceneScaleArg.getValue();

            bool sceneLoadOk = SceneLoader::load(r->scenePath.c_str(), r);
            if( sceneLoadOk == false)
            {
                std::cout << "Could not load scene file!" << std::endl;
                return false;
            }
            for (size_t i = 0; i < r->meshPaths.size(); ++i) 
            {
                string meshName = r->meshPaths[i];
                r->meshPaths[i].append(".mesh.xml");
                if(Utils::checkIfFileExists(r->meshPaths[i].c_str()) == false)
                {
                    std::cout << "Mesh file not found!" << std::endl;
                    return false;
                }

                std::string dPath = meshName;
                dPath.append(".mesh.dat");
                r->dataPaths.push_back(dPath);
                
                if(Utils::checkIfFileExists(r->dataPaths[i].c_str()) == false)
                {
                    std::cout << "Binary file not found!" << std::endl;
                    return false;
                }
                
//                r->sceneDir.append(meshName+"/");
//                r->sceneDir.append();

                size_t posSlash = r->meshPaths[i].find_last_of("/");
                if(posSlash == std::string::npos)
                {
                    r->texPaths.push_back("./");
                }
                else
                {
                    r->texPaths.push_back(r->meshPaths[i].substr(0, posSlash+1));
                }
                string animPath = meshName+"Action.anim.xml";
                if(Utils::checkIfFileExists(animPath.c_str()))
                {
                    r->animMetas.push_back(animPath);
                    size_t pos = r->animMetas.back().find(".xml");
                    std::string animPathData = r->animMetas.back().substr(0, pos);
                    animPathData.append(".dat");
                    if(Utils::checkIfFileExists(animPathData.c_str()) )
                    {
                        r->animDatas.push_back(animPathData);
                    }
                    else
                    {
                        r->animDatas.push_back(string());
                        return false;
                    }
                }
            }
            
        }
        else 
        {
            r->hasScene = false;
            r->numObj = 1;
            r->meshPaths.push_back(metaFileArg.getValue());
            r->names = std::vector<std::string>(1,"");
            r->sceneScale = 1.0f;
            r->animMetas = std::vector<std::string>(1,"");
            r->animDatas = std::vector<std::string>(1,"");
            
            if(Utils::checkIfFileExists(r->meshPaths[0].c_str()) == false)
            {
                cout << "Mesh file not found!" << endl;
                return false;
            }

//            size_t posext = r->meshPaths[0].find(".xml");
            std::string meshName = r->meshPaths[0].substr(0, r->meshPaths[0].find(".mesh.xml"));
            size_t posslash = meshName.find_last_of("/");

            if(posslash == std::string::npos)
            {
                r->names[0] = meshName;
            }
            else
            {
                r->names[0] = meshName.substr(posslash+1);
            }


            if(dataFileArg.isSet())
            {
                r->dataPaths.push_back(dataFileArg.getValue());
            }
            else
            {
                size_t pos = r->meshPaths[0].find(".xml");
                std::string dPath = r->meshPaths[0].substr(0, pos);
                dPath.append(".dat");
                r->dataPaths.push_back(dPath);
            }
            if(Utils::checkIfFileExists(r->dataPaths[0].c_str()) == false)
            {
                std::cout << "Binary file not found!" << std::endl;
                return false;
            }
            
            r->scales.push_back(meshScaleArg.getValue());
            
            size_t pos = r->meshPaths[0].find_last_of("/");
            if(pos == std::string::npos)
            {
                r->texPaths.push_back("./");
            }
            else
            {
                r->texPaths.push_back(r->meshPaths[0].substr(0, pos+1));
            }
            std::vector<float> tmpPos(3, 0.0f);
            r->positions.push_back(tmpPos);
            std::vector<float> tmpOrientation(3, 0.0f);
            r->orientations.push_back(tmpOrientation);
            if(animArg.isSet())
            {
                std::string animPathMeta = animArg.getValue();
                if(Utils::checkIfFileExists(animPathMeta.c_str()) )
                {
                    r->animMetas[0] = animPathMeta;
                }
                else
                {
                    r->animMetas[0] = string();
                    return false;
                }
                size_t pos = r->animMetas[0].find(".xml");
                std::string animPathData = r->animMetas[0].substr(0, pos);
                animPathData.append(".dat");
                r->animDatas[0] = animPathData;
                if( ! Utils::checkIfFileExists(animPathData.c_str()) )
                {
                    r->animDatas[0] = string();
                    return false;
                }
            }

        }
        return true;

    }
    catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return false;
    }
}
