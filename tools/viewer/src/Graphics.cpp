/*
 * Copyright (c) 2011 Peter Vasil, Micheal Nischt
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

#include "Graphics.h"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gli/gli.hpp"
#include "gli/gtx/gl_texture2d.hpp"

#include <GL/glew.h>

using namespace std;

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
    for(vector<Mesh*>::iterator it=meshes.begin(); it != meshes.end(); ++it)
    {
        delete *it;
    }
    meshes.clear();
    textures.clear();
    shapes.clear();

    if(simple) delete simple; simple = NULL;
}

void Graphics::init()
{


    glClearColor(0.25f,0.25f,0.25f,1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);

    Location3D* loc = new Location3D();
    loc->x = 2.0f;
//    loc->rotY = 1.0f;
    loc->rotAngle = 0.0f;
    Mesh* m = loadMesh(resources.meshPath.c_str(), resources.dataPath.c_str());
    for (int i = 0; i < resources.textureNames.size(); ++i) {
        std::string tPath = resources.texPath + resources.textureNames[0];
        Texture t = loadTexture(tPath.c_str());
    }
//    Texture t = loadTexture("data/crate.tga");
//    addObject(loc, m, t, 4.0f);
    addObject(loc, m, 1.0f);

    simple = new ProgramSimple();
}

void Graphics::render(int width, int height)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(simple->programName());

    // here implemetation
    float camZ = 15.0f;
    float near = 3.0f; float far = 100.0f;
    float aspect = width/(GLfloat)height;

    glm::mat4 V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -camZ));
    glm::mat4 P = glm::frustum(-1.0f,1.0f, -1.0f, 2.0f/aspect-1.0f, near, far);
    simple->projection(glm::value_ptr(P));

    for(unsigned int i = 0; i < shapes.size(); ++i)
    {
        Location3D *loc = shapes[i].location;

        glm::mat4 M = glm::mat4(1.0f);

        M = glm::translate(M, glm::vec3(loc->x, loc->y, loc->z));
        M = glm::rotate(M, loc->rotAngle * 180.0f/3.14f, glm::vec3(loc->rotX, loc->rotY, loc->rotZ));
        M = glm::scale(M, glm::vec3(shapes[i].scale, shapes[i].scale, shapes[i].scale));

        glm::mat4 MV = V*M;
        simple->modelView(glm::value_ptr(MV));
        for (int j = 0; j < shapes[i].textures.size(); ++j) {
            glBindTexture(GL_TEXTURE_2D, shapes[i].textures[j]);
            shapes[i].mesh->draw(j);
            glBindTexture(GL_TEXTURE_2D, 0);

        }
    }

    glUseProgram(0);

}

Mesh* Graphics::loadMesh(const char* meta, const char* data)
{
    Mesh* mesh = new Mesh(meta, data);
    meshes.push_back(mesh);
    for (int i = 0; i < mesh->getNGroups(); ++i) {
//        printf("%s\n", mesh->getGroupName(i));
        resources.textureNames.push_back(mesh->getGroupName(i));
    }

    return mesh;
}

Texture Graphics::loadTexture(const char* texName)
{
    Texture texture = gli::createTexture2D(texName);
    textures.push_back(texture);
    return texture;
}

void Graphics::setResources(Resources r)
{
    resources = r;
}

void Graphics::addObject(Location3D *loc, Mesh *mesh, Texture texture, float scale)
{
    Shape3D shape;
    shape.location = loc;
    shape.texture = texture;
    shape.mesh = mesh;
    shape.scale = scale;
    shapes.push_back(shape);
}

void Graphics::addObject(Location3D *loc, Mesh *mesh, float scale)
{
    Shape3D shape;
    shape.location = loc;
    shape.textures = textures;
    shape.mesh = mesh;
    shape.scale = scale;
    shapes.push_back(shape);
}
