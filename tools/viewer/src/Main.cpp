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

#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>

//#include "Graphics.h"
#include "App.h"
#include "Settings.h"


// Globals
struct MouseInfo
{
    int x,y;
    bool left, right;
} mouse;
int runLevel = 1;
bool keys[GLFW_KEY_LAST] = {false};  // Key monitor

//Graphics graphics;
App app;

// Initializationa
void initWindow(int scrX, int scrY, int BPP);

// Event callback functions
void keyCallback(int key, int action);
void mouseButtonCallback(int button, int action);
void mousePosCallback(int x, int y);
void resized(int width, int height);
void mouseWheel(int pos);

void updateView();

// process keys
void processKeys(double deltaTime);
void processMouse(double deltaTime);

int winWidth = 800;
int winHeight = 600;

float lastx = 0;
float lasty = 0;
float camRotX = 0;
float camRotY = 0;
float camOffsetX = 0.0f;
float camOffsetY = 0.0f;
float camOffsetZ = 25.0f;
float lastWheel = 0.0f;

int main(int argc, char *argv[])
{
    //----------------------------------------
    // Cmd argument parsing
    //----------------------------------------
    Resources r;
		if(!loadSettings(r, argc, argv))
			return 1;
		
    //----------------------------------------
    // Start loop
    //----------------------------------------
    double time = glfwGetTime();
    int retval = 0;
    try
    {
        // Initialize the window
        initWindow(winWidth, winHeight, 32);
        
        //----------------------------------------
        // Initializing app
        //----------------------------------------
        app.init(r);

        //----------------------------------------

        while(runLevel)
        {
            if(keys[GLFW_KEY_ESC] || glfwGetWindowParam(GLFW_OPENED) != GL_TRUE) // Esc Key
                runLevel=0;
            
            double currentTime = glfwGetTime();
            double dT = currentTime - time;
            processKeys(dT);
            processMouse(dT);
            time = currentTime;

            app.render(winWidth, winHeight);

            // We're using double buffers, so we need to swap to see our stuff
				    glfwSwapBuffers();
	        }
        
    }
    catch (const char* error)
    {
        // Report an error
        std::ofstream errorFile("GL_ERROR.txt");
        errorFile << "Caught exception:\n  " << error << '\n';
        retval = 1;
    }

    // Shut down GLFW
    glfwTerminate();

    // Return the appropriate value
    return retval;
}

// Initialize the window, can throw if something goes wrong.
void initWindow(int scrX, int scrY, int BPP)
{
//    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
//    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    // Initialize the GLFW library
    if (glfwInit() != GL_TRUE)
        throw "Failed to initialize GLFW.";
    
    // Create a window (8-bit depth-buffer, no alpha and stencil buffers, windowed)
    if (glfwOpenWindow(scrX, scrY, BPP/3, BPP/3, BPP/3, 0, 8, 0, GLFW_WINDOW) != GL_TRUE)
        throw "Failed to open window.";
    
    // Give the window a title
    glfwSetWindowTitle("Assembly3D viewer");
    
    // Register event callbacks
    glfwSetKeyCallback(keyCallback);
    glfwSetMouseButtonCallback(mouseButtonCallback);
    glfwSetMousePosCallback(mousePosCallback);
    glfwSetWindowSizeCallback(resized);
    glfwSetMouseWheelCallback(mouseWheel);
    
//    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw "Could not init GLEW!";

}


// Handle keys - updates the Keys array
void keyCallback(int key, int action)
{
//    std::cout << key << std::endl;
    keys[key] = (action == GLFW_PRESS);
    
    if (keys[GLFW_KEY_ESC])
        runLevel = 0;
}
bool firstDragL = false;
bool firstDragR = false;
// Handle mouse button events - updates the Mouse structure
void mouseButtonCallback(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        mouse.left = (action == GLFW_PRESS);
        firstDragL = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        mouse.right = (action == GLFW_PRESS);
        firstDragR = true;
    }
}

// Handle mouse motion - updates the Mouse structure
void mousePosCallback(int x, int y)
{
    mouse.x = x;
    mouse.y = y;
}

void resized(int width, int height)
{
    winWidth = width;
    winHeight = height;
}

void mouseWheel(int pos)
{
    float diffWheel = pos - lastWheel;
    camOffsetZ -= diffWheel;
    lastWheel = pos;
}

void updateView()
{
    app.updateView(camOffsetX, camOffsetY, camOffsetZ, camRotX, camRotY);
}

void processKeys(double deltaTime)
{
    if(keys[GLFW_KEY_UP] || keys[87])
    {
        camOffsetZ -= 50.0f*(float)deltaTime;
        updateView();
    }
    if(keys[GLFW_KEY_DOWN] || keys[83])
    {
        camOffsetZ += 50.0f*(float)deltaTime;
        updateView();
    }
}

void processMouse(double deltaTime)
{
    if(mouse.left)
    {
        if(firstDragL)
        {
            lastx=mouse.x;
            lasty=mouse.y;
            firstDragL = false;
        }
        int diffx=mouse.x-lastx;
        int diffy=mouse.y-lasty;
        camRotX += (float) (diffy * 200 * deltaTime);
        camRotY += (float) (diffx * 200 * deltaTime);
        lastx=mouse.x;
        lasty=mouse.y;
        //updateView();
    }
    if(mouse.right)
    {
        if(firstDragR)
        {
            lastx = mouse.x;
            lasty = mouse.y;
            firstDragR = false;
            
        }
        int diffx=mouse.x-lastx;
        int diffy=mouse.y-lasty;
        camOffsetX -= (float) (diffx * 20 * deltaTime);
        camOffsetY += (float) (diffy * 20 * deltaTime);
        lastx=mouse.x;
        lasty=mouse.y;
        //updateView();
    }
    updateView();

    
}
