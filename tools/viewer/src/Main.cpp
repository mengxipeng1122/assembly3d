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
#include <tclap/CmdLine.h>


// Globals
struct MouseInfo
{
    int x,y;
    bool left, right;
} mouse;
int runLevel = 1;
bool keys[GLFW_KEY_LAST] = {false};  // Key monitor

float angleY = 0.0f;
float viewOffset = 25.0f;

//Graphics graphics;
App app;
const char* vertexShaderPathStr = "glsl/Simple.vert";
const char* fragmentShaderPathStr = "glsl/Simple.frag";

const char* metaPath = "";
const char* dataPath = "";
const char* texFolder = "";

// Initializationa
void initWindow(int scrX, int scrY, int BPP);

// Event callback functions
void keyCallback(int key, int action);
void mouseButtonCallback(int button, int action);
void mousePosCallback(int x, int y);
void resized(int width, int height);

// Swap buffers
void flipBuffers();

// process keys
void processKeys(double deltaTime);

int winWidth = 800;
int winHeight = 600;

int main(int argc, char *argv[])
{
    //----------------------------------------
    // Cmd argument parsing
    //----------------------------------------
    Resources r;

    try {
        using namespace TCLAP;
        CmdLine cmd("Assembly3D Viewer", '=', "1.0.0");

        UnlabeledValueArg<std::string> metaFileArg("input-file", "Input file.", true, "", "path/to/mesh");
        ValueArg<std::string> dataFileArg("b", "binary-file", "Binary file.", false, "", "path/to/binary");
        ValueArg<float> meshScaleArg("s", "scale", "Scale.", false, 1.0f, "scale");

        cmd.add(meshScaleArg);
        cmd.add(dataFileArg);
        cmd.add(metaFileArg);

        cmd.parse(argc, argv);

        r.meshPath = metaFileArg.getValue();
        if(dataFileArg.isSet())
        {
            r.dataPath = dataFileArg.getValue();
        }
        else
        {
            size_t pos = r.meshPath.find(".xml");
            r.dataPath = r.meshPath.substr(0, pos);
            r.dataPath.append(".dat");
        }
        r.scale = meshScaleArg.getValue();

        size_t pos = r.meshPath.find_last_of("/");
        if(pos == std::string::npos)
        {
            r.texPath = "./";
        }
        else
        {
            r.texPath = r.meshPath.substr(0, pos+1);
        }

    } catch (TCLAP::ArgException &e)  // catch any exceptions
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; return 1;}
    //----------------------------------------
    // Start loop
    //----------------------------------------
    double time = glfwGetTime();
    int retval = 0;
    try
    {
        // Initialize the window
        initWindow(winWidth, winHeight, 32);
        // Pass control to the render function
        
        //----------------------------------------
        // Initializing app
        //----------------------------------------

        app.init(r);

        //----------------------------------------

        while(runLevel)
        {
            if(keys[GLFW_KEY_ESC]) // Esc Key
                runLevel=0;
            
            double currentTime = glfwGetTime();
            double dT = currentTime - time;
            processKeys(dT);
            app.update(dT);
            time = currentTime;

            app.render(winWidth, winHeight);

            // We're using double buffers, so we need to swap to see our stuff
            flipBuffers();
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
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

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
    
//    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw "Could not init GLEW!";

}

// Wrapper for buffer swapping
void flipBuffers()
{
    glfwSwapBuffers();
    // glfwSwapBuffers also automatically polls for input
    
    // If the window was closed we quit
    if (glfwGetWindowParam(GLFW_OPENED) != GL_TRUE)
        runLevel = 0;

}

// Handle keys - updates the Keys array
void keyCallback(int key, int action)
{
    keys[key] = (action == GLFW_PRESS);
    
    if (keys[GLFW_KEY_ESC])
        runLevel = 0;
}

// Handle mouse button events - updates the Mouse structure
void mouseButtonCallback(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        mouse.left = (action == GLFW_PRESS);
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        mouse.right = (action == GLFW_PRESS);
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
    glViewport(0, 0, width, height);
}

void updateView()
{
    app.updateView(viewOffset,angleY);
}

void processKeys(double deltaTime)
{
    if(keys[GLFW_KEY_LEFT])
    {
        angleY -= 1.0f*(float)deltaTime;
        updateView();
    }
    if(keys[GLFW_KEY_RIGHT])
    {
        angleY += 1.0f*(float)deltaTime;
        updateView();
    }
    if(keys[GLFW_KEY_UP])
    {
        viewOffset -= 50.0f*(float)deltaTime;
        updateView();
    }
    if(keys[GLFW_KEY_DOWN])
    {
        viewOffset += 50.0f*(float)deltaTime;
        updateView();
    }
}
