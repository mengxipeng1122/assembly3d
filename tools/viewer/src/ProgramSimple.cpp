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

#include "ProgramSimple.h"
#include "ShaderUtils.h"

#include <algorithm>

//#include "glm/glm.hpp"
//#include "glm/gtc/type_ptr.hpp"
//#include "glm/gtc/matrix_transform.hpp"

ProgramSimple::ProgramSimple()
{
    GLuint vs = ShaderUtils::createVertexShader("glsl/Simple.vert");
    GLuint fs = ShaderUtils::createFragmentShader("glsl/Simple.frag");

    program = ShaderUtils::createProgram(vs, fs);

    projectionLoc = glGetUniformLocation(program, "projection");
    modelViewLoc = glGetUniformLocation(program, "modelView");
}

ProgramSimple::~ProgramSimple()
{
    if(glIsProgram(program))
    {
        glDeleteProgram(program);
        program = 0;
    }
}

GLuint ProgramSimple::programName()
{
    return program;
}

void ProgramSimple::projection(float *matrix)
{
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, matrix);
}

void ProgramSimple::modelView(float *matrix)
{
    glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, matrix);
//    glm::mat4 m = glm::mat4(1.0f);
//    std::copy(matrix, matrix+16, glm::value_ptr(m));
//    glm::mat4 invT = glm::transpose(glm::inverse(m));
//    glUniformMatrix4fv(lightingLoc, 1, GL_FALSE, glm::value_ptr(invT));

}



