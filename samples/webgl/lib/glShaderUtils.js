/*
 * Copyright (c) 2011 Michael Nischt
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

function createVertexShader(gl, str) 
{
	return createShader(gl, str, gl.VERTEX_SHADER);
}

function createFragmentShader(gl, str) 
{
	return createShader(gl, str, gl.FRAGMENT_SHADER);
}

function createShader(gl, str, shaderType) 
{   
    var shader = gl.createShader(shaderType);

    gl.shaderSource(shader, str);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) 
    {
        alert(gl.getShaderInfoLog(shader));
        return null;
    }

    return shader;
}

function createProgram(gl, vertexShader, fragmentShader) 
{
  var shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) 
  {
    alert("Could not initialise shaders");
		return null;
  }
	
	return shaderProgram;
}




function getProgramAttributes(gl, program, attributes)
{
	if(!attributes) attributes = {};

	var count = gl.getProgramParameter(program, gl.ACTIVE_ATTRIBUTES);
	
	for(var i=0; i<count; i++)
	{
		var info = gl.getActiveAttrib(program, i);
		attributes[info.name] = gl.getAttribLocation(program, info.name);
	}

  return attributes;
}

function getProgramUniforms(gl, program, uniforms)
{
	if(!uniforms) uniforms = {};

	var count = gl.getProgramParameter(program, gl.ACTIVE_UNIFORMS);
	
	for(var i=0; i<count; i++)
	{
		var info = gl.getActiveUniform(program, i);
		uniforms[info.name] = gl.getUniformLocation(program, info.name);
	}

  return uniforms;
}

