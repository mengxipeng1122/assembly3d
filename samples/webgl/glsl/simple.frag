#ifdef GL_ES
precision highp float;
#endif

const vec3 LightDirection = vec3(0.23570226039551587, 0.23570226039551587, 0.9428090415820635); //in eye-space

uniform sampler2D Sampler;

varying vec3 _Normal;
varying vec2 _TexCoord;

void main(void) 
{
	float _Intensity = max(dot(normalize(_Normal), LightDirection), 0.0);
  vec4 texColor = texture2D(Sampler, _TexCoord.st);
  gl_FragColor = vec4(texColor.rgb * _Intensity, texColor.a);
}
