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
#include "Location3D.h"
#include "MeshLoader.h"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#ifdef A3D_USE_FREEIMAGE
#include "FreeImage.h"
#endif

using namespace std;

Graphics::Graphics()
    : eyeX(0.0f),
      eyeY(0.0f),
      eyeZ(25.0f),
      rotX(0.0f),
      rotY(0.0f)
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

    if(simple)
    {
        delete simple; 
        simple = NULL;
    }
}

void Graphics::init()
{

    glClearColor(0.25f, 0.25f, 0.25f, 0.25f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
//    glEnable(GL_MULTISAMPLE);

    simple = new ProgramSimple();

}

void Graphics::render(int width, int height)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glViewport(0, 0, width, height);
    
    glUseProgram(simple->programName());
    glUniform1i(simple->texCoord(), 0);

//    float viewOffset = 25.0f;
//    float camZ = 25.0f;
//    float near = 3.0f; float far = 100.0f;
    float aspect = width/(GLfloat)height;

//    glm::mat4 V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -camZ));
//    glm::mat4 P = glm::frustum(-1.0f,1.0f, -1.0f, 2.0f/aspect-1.0f, near, far);
//    0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
    glm::mat4 V = glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ),
                             glm::vec3(eyeX, eyeY, 0.0f),
                             glm::vec3(0.0f, 1.0f, 0.0f));

//    V = glm::translate(V, glm::vec3(0.0, 5.0, 0.0));
    V = glm::rotate(V, rotX, glm::vec3(1.0f, 0.0f, 0.0f));
    V = glm::rotate(V, rotY, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 P = glm::perspective(45.0f, aspect, 0.1f, 128.0f);

    simple->projection(glm::value_ptr(P));

    for(unsigned int i = 0; i < shapes.size(); ++i)
    {
        Location3D *loc = shapes[i].location;

        glm::mat4 M = glm::mat4(1.0f);

        M = glm::translate(M, glm::vec3(loc->x, loc->y, loc->z));
        M = glm::rotate(M, loc->rotAngle, glm::vec3(loc->rotX, loc->rotY, loc->rotZ));
        M = glm::scale(M, glm::vec3(shapes[i].scale, shapes[i].scale, shapes[i].scale));

        glm::mat4 MV = V*M;
        simple->modelView(glm::value_ptr(MV));
        for (unsigned int j = 0; j < shapes[i].textures.size(); ++j) {
            glBindTexture(GL_TEXTURE_2D, shapes[i].textures[j]);
            shapes[i].mesh->draw(j);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    glUseProgram(0);

}

Mesh* Graphics::loadMesh(const char* meta, const char* data)//, Resources& r)
{
    Mesh* mesh = new Mesh(simple);
    MeshLoader ml(mesh, meta, data, simple);
    meshes.push_back(mesh);
    return mesh;
}

#ifdef A3D_USE_FREEIMAGE
static FIBITMAP* GenericLoader(const char* lpszPathName, int flag = 0) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(lpszPathName, 0);
	if(fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
        // ok, let's load the file
		FIBITMAP *dib = FreeImage_Load(fif, lpszPathName, flag);
		// unless a bad file format, we are done !
		return dib;
	}
	return NULL;
}
#endif

Texture Graphics::loadTexture(const char* texName)
{
    GLFWimage img;
    GLint internalImgFormat;

#ifdef A3D_USE_FREEIMAGE
    FIBITMAP *bitmap = GenericLoader(texName);
    if(bitmap)
    {
        img.Width = FreeImage_GetWidth(bitmap);
        img.Height = FreeImage_GetHeight(bitmap);
        
        bitmap = FreeImage_ConvertTo32Bits(bitmap);
        img.Format = GL_BGRA;
        internalImgFormat = GL_RGBA;
        img.BytesPerPixel = 4;
        img.Data = new unsigned char[img.BytesPerPixel*img.Width*img.Height];
        char* pixels = (char*)FreeImage_GetBits(bitmap);
        memcpy(img.Data, pixels, img.BytesPerPixel*img.Width*img.Height);
        //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
//        for(int j= 0; j<img.Width*img.Height; j++){
//            img.Data[j*4+0]= pixels[j*4+2];
//            img.Data[j*4+1]= pixels[j*4+1];
//            img.Data[j*4+2]= pixels[j*4+0];
//            img.Data[j*4+3]= pixels[j*4+3];
//        }
        FreeImage_Unload(bitmap);
    }
#else
    if(glfwReadImage(texName, &img, GLFW_BUILD_MIPMAPS_BIT))
    {
        internalImgFormat = img.Format;
    }
#endif
    else
    {
        float r, g, b;
        r = b = 120;
        g = 200;
        img.Format = GL_RGB;
        internalImgFormat = img.Format;
        img.Width = 1;
        img.Height = 1;
        img.Data = new unsigned char[img.Width*img.Height*3];
        img.Data[0] = (unsigned char)r;
        img.Data[1] = (unsigned char)g;
        img.Data[2] = (unsigned char)b;
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, internalImgFormat, img.Width, img.Height, 0, img.Format, GL_UNSIGNED_BYTE, img.Data);
    
    // texture parameters
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    
    glfwFreeImage(&img);

    textures.push_back(texture);
    return texture;
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

void Graphics::updateView(float eyex, float eyey, float eyez)
{
    eyeX = eyex;
    eyeY = eyey;
    eyeZ = eyez;
}

void Graphics::updateView(float xrot, float yrot)
{
    rotX = xrot;
    rotY = yrot;
}
