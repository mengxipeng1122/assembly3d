//#version 110

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

//in 
attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 texCoord;

//out
varying vec3 fragmentNormal;
varying vec2 fragmentTexCoord;

void main(void)
{
	fragmentTexCoord = texCoord;

    vec4 n = modelViewMatrix*vec4(normal, 0.0);
	fragmentNormal = n.xyz;

	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
