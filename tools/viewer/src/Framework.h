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

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <GL/glew.h>
#include <GL/glfw.h>

#include "Vector2.h"
#include "Vector3.h"

#include "Mesh.h"

// Functions
void flipBuffers();
void render();

//void loadShaders();

// A struct to hold mouse data
struct MouseInfo
{
    int x,y;
    bool left, right;
};

// Globals
extern int runLevel;
extern bool keys[GLFW_KEY_LAST];  // Key monitor
extern MouseInfo mouse; // Mouse monitor

extern const char* vertexShaderPathStr;
extern const char* fragmentShaderPathStr;

extern const char* metaPath;
extern const char* dataPath;

extern GLuint g_shaderProgram;

extern Mesh* mesh;

extern GLint projectionLocation;
extern GLint modelViewLocation;
extern GLint vertexLocation;
extern GLint normalLocation;
extern GLint texCoordLocation;
extern GLint textureLocation;


// Definitions of some windows key codes in terms of GLFW keys (Use
// uppercase characters for characer keys - 'X' is the x key for
// example.
const unsigned int
VK_ESCAPE = GLFW_KEY_ESC,
VK_RETURN = GLFW_KEY_ENTER,
VK_SPACE = GLFW_KEY_SPACE,
VK_UP = GLFW_KEY_UP,
VK_DOWN = GLFW_KEY_DOWN,
VK_RIGHT = GLFW_KEY_RIGHT,
VK_LEFT = GLFW_KEY_LEFT,
VK_HOME = GLFW_KEY_HOME,
VK_END = GLFW_KEY_END,
VK_INSERT = GLFW_KEY_INSERT,
VK_DELETE = GLFW_KEY_DEL;

#endif // FRAMEWORK_H
