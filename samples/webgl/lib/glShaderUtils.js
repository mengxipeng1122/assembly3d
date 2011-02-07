/*
 * Copyright (c) 2011 Michael Nischt
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source
 *    distribution.
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

