const vec3 LightDirection = vec3(0.23570226039551587, 0.23570226039551587, 0.9428090415820635); //in eye-space

uniform mat4 Projection;
uniform mat4 ModelView;
uniform mat4 Lighting;

attribute vec3 Position;
attribute vec3 Normal;
attribute vec2 TexCoord;

varying vec2 _TexCoord;
varying float _Intensity;

void main(void) 
{
		gl_Position = Projection * ModelView * vec4(Position, 1.0);
    _TexCoord = TexCoord;

		vec4 eyeNormal = Lighting * vec4(Normal, 0.0);
    _Intensity = max(dot(normalize(eyeNormal.xyz), LightDirection), 0.0);;
}
