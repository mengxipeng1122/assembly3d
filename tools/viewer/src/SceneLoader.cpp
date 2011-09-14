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

#include "SceneLoader.h"

#include "Resources.h"
#include <iostream>
#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using namespace std;

static void processSceneNode(xmlTextReaderPtr reader, Resources* r);

SceneLoader::SceneLoader()
{
}

bool SceneLoader::load(const char *scenePath, Resources* r)
{
//    bool loadOk;

    xmlTextReaderPtr reader = xmlReaderForFile(scenePath, NULL, 0);
    if(reader == NULL)
    {
        return false;
    }
    int ret = xmlTextReaderRead(reader);
    while (ret == 1)
    {
        processSceneNode(reader, r);
        ret = xmlTextReaderRead(reader);
    }
    xmlFreeTextReader(reader);
    assert(ret == 0);

    return true;
}

static void processSceneNode(xmlTextReaderPtr reader, Resources* r)
{
    const xmlChar* name = xmlTextReaderConstName(reader);
    assert (name != NULL);

    int nodeType = xmlTextReaderNodeType(reader);
    if(nodeType != 1)
    {
        return;
    }

    if(strcmp("World", (const char*)name) == 0)
    {
        int numObjects = 0;
        xmlChar* numVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "objects");
        if(numVal)
            numObjects = atoi ((const char*) numVal);
        r->numObj = numObjects;
        xmlFree(numVal);
    }
    else if(strcmp("Object", (const char*)name) == 0)
    {
        xmlChar* objName = xmlTextReaderGetAttribute(reader, (xmlChar*) "name");
        string meshPath;
        meshPath = r->sceneDir;
        if(objName)
            meshPath.append((const char*)objName);
        r->meshPaths.push_back(meshPath);
        r->names.push_back((const char*)objName);

        float scale = 1.0f;
        xmlChar* attribScale = xmlTextReaderGetAttribute(reader, (xmlChar*) "scale");
        if(attribScale)
            scale = atof ((const char*) attribScale );

        r->scales.push_back(scale);

        xmlFree(objName);
        xmlFree(attribScale);
    }
    else if(strcmp("Position", (const char*)name) == 0)
    {
        vector<float> positionValues(3, 0.0f);
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
        xmlFree(xVal);
        xmlFree(yVal);
        xmlFree(zVal);
    }
    else if(strcmp("Orientation", (const char*)name) == 0)
    {
        vector<float> orientationValues(3, 0.0f);
        xmlChar* xVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "x");
        xmlChar* yVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "y");
        xmlChar* zVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "z");
        xmlChar* wVal = xmlTextReaderGetAttribute(reader, (xmlChar*) "w");

        if(xVal)
            orientationValues[0] = atof((const char*) xVal );
        if(yVal)
            orientationValues[1] = atof((const char*) yVal );
        if(zVal)
            orientationValues[2] = atof((const char*) zVal );
        if(wVal)
            orientationValues.push_back(atof((const char*) wVal ));

        r->orientations.push_back(orientationValues);
        xmlFree(xVal);
        xmlFree(yVal);
        xmlFree(zVal);
        xmlFree(wVal);
    }
}
