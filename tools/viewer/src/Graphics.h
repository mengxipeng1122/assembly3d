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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>

#include "Mesh.h"
#include "ProgramSimple.h"
#include "Resources.h"

//#include <string>
#include <vector>

typedef GLuint Texture;

struct Location3D
{
    Location3D() : x(0), y(0), z(0), rotX(0), rotY(0), rotZ(1), rotAngle(0) {}
    float x, y, z;
    float rotX, rotY, rotZ, rotAngle;
};

class Graphics
{
public:
    Graphics();
    ~Graphics();
    
    void setResources(Resources r);
    void init();
    void render(int width, int height);
    
    Mesh* loadMesh(const char* meta, const char* data);
    Texture loadTexture(const char* texName);
    void addObject(Location3D* loc, Mesh* mesh, Texture texture, float scale);
    void addObject(Location3D* loc, Mesh* mesh, float scale);

    
private:

    Resources resources;

    struct Shape3D
    {
        std::vector<Texture> textures;
        Texture texture;
        Mesh* mesh;
        Location3D *location;
        float scale;
    };

    std::vector<Mesh*> meshes;
    std::vector<Texture> textures;
    std::vector<Shape3D> shapes;

    ProgramSimple* simple;
};

#endif // GRAPHICS_H
