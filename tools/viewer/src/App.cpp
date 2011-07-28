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

#include "App.h"
#include "Location3D.h"
#include <cmath>

App::App()
{
}

App::~App()
{

}

void App::init(Resources r)
{
    graphics = new Graphics();

    graphics->init();

    Location3D* loc = new Location3D();
//    loc->x = 2.0f;
//    loc->y = -1.0f;
//    loc->rotY = 1.0f;
//    loc->rotAngle = 60.0f;
    Mesh* mesh = graphics->loadMesh(r.meshPath.c_str(), r.dataPath.c_str());
    for (int i = 0; i < mesh->getNGroups(); ++i) {
        std::string tPath = r.texPath + std::string(mesh->getGroupName(i));
        graphics->loadTexture(tPath.c_str());
    }
    graphics->addObject(loc, mesh, r.scale);

}

void App::render(int width, int height)
{
    graphics->render(width, height);
}

void App::update(float deltaTime)
{
}

void App::updateView(float offset, float angleY)
{
    graphics->updateView(offset * sin(angleY), 0.0f, offset * cos(angleY));
}

