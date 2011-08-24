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
#include <vector>
#include <libxml/xmlreader.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using namespace std;
using namespace TCLAP;

static void processNode(xmlTextReaderPtr reader, Resources* r);

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

        cmd.add(sceneScaleArg);
        cmd.add(meshScaleArg);
        cmd.add(dataFileArg);
        cmd.add(metaFileArg);
        cmd.add(meshDirArg);
        cmd.add(sceneFileArg);

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
            
            if(meshDirArg.isSet())
            {
                string meshDir = meshDirArg.getValue();
                char lastChar = meshDir.at(meshDir.length()-1);
                if(lastChar != '/')
                    meshDir.append("/");
                r->sceneDir = meshDir;
            }
            else
            {
                size_t pos = r->scenePath.find_last_of("/");
                if(pos == std::string::npos)
                {
                    r->sceneDir = "./";
                }
                else
                {
                    r->sceneDir = r->scenePath.substr(0, pos+1);
                }

            }
            
            if(Utils::checkIfFileExists(r->scenePath.c_str()) == false)
            {
                cout << "Scene file not found!" << endl;
                return false;
            }
            
            r->sceneScale = sceneScaleArg.getValue();

            // parse scene file
            xmlTextReaderPtr reader = xmlReaderForFile(r->scenePath.c_str(), NULL, 0);
            assert(reader != NULL);
            int ret = xmlTextReaderRead(reader);
            while (ret == 1)
            {
                processNode(reader, r);
                ret = xmlTextReaderRead(reader);
            }
            xmlFreeTextReader(reader);
            assert(ret == 0);
            
            for (size_t i = 0; i < r->meshPaths.size(); ++i) 
            {
                size_t posExt = r->meshPaths[i].find(".xml");
                std::string dPath = r->meshPaths[i].substr(0, posExt);
                dPath.append(".dat");
                r->dataPaths.push_back(dPath);
                
                if(Utils::checkIfFileExists(r->dataPaths[i].c_str()) == false)
                {
                    std::cout << "Binary file not found!" << std::endl;
                    return false;
                }
                
                size_t posSlash = r->meshPaths[i].find_last_of("/");
                if(posSlash == std::string::npos)
                {
                    r->texPaths.push_back("./");
                }
                else
                {
                    r->texPaths.push_back(r->meshPaths[i].substr(0, posSlash+1));
                }
                
            }
            return true;
            
        }
        else 
        {
            r->hasScene = false;
            r->numObj = 1;
            r->meshPaths.push_back(metaFileArg.getValue());
            r->sceneScale = 1.0f;
            
            if(Utils::checkIfFileExists(r->meshPaths[0].c_str()) == false)
            {
                cout << "Mesh file not found!" << endl;
                return false;
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

            return true;
        }
    }
    catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return false;
    }
}

static void processNode(xmlTextReaderPtr reader, Resources* r)
{
    const char *name = (const char*)xmlTextReaderConstName(reader);
    assert (name != NULL);
    
    int nodeType = xmlTextReaderNodeType(reader);
    if(nodeType != 1)
    {
        return;
    }

    if(strcmp("World", name) == 0)
    {
        int numObjects = 0;
        xmlChar* numVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "objects");
        if(numVal)
            numObjects = atoi ((const char*) numVal);
        r->numObj = numObjects;
    }
    else if(strcmp("Object", name) == 0)
    {
        const char* name = (const char*) xmlTextReaderGetAttribute(reader, (xmlChar*) "name");
        string meshPath;
        meshPath = r->sceneDir;
        if(name)
            meshPath.append(name);
        r->meshPaths.push_back(meshPath);

        float scale = 1.0f;
        xmlChar* attribScale = xmlTextReaderGetAttribute(reader, (xmlChar*) "scale");
        if(attribScale)
            scale = atof ((const char*) attribScale );

        r->scales.push_back(scale);
        
    }
    else if(strcmp("Position", name) == 0)
    {
        std::vector<float> positionValues(3, 0.0f);
        xmlChar* xVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "x");
        xmlChar* yVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "y");
        xmlChar* zVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "z");

        if(xVal)
            positionValues[0] = atof((const char*) xVal );
        if(yVal)
            positionValues[1] = atof((const char*) yVal );
        if(zVal)
            positionValues[2] = atof((const char*) zVal );

        r->positions.push_back(positionValues);
    }
    else if(strcmp("Orientation", name) == 0)
    {
        std::vector<float> orientationValues(3, 0.0f);
        xmlChar* xVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "x");
        xmlChar* yVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "y");
        xmlChar* zVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "z");

        if(xVal)
            orientationValues[0] = atof((const char*) xVal );
        if(yVal)
            orientationValues[1] = atof((const char*) yVal );
        if(zVal)
            orientationValues[2] = atof((const char*) zVal );

        r->orientations.push_back(orientationValues);
    }
}
