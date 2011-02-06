#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D Sampler;

varying vec2 _TexCoord;
varying float _Intensity;

void main(void) 
{
    vec4 texColor = texture2D(Sampler, _TexCoord.st);
    gl_FragColor = vec4(texColor.rgb * _Intensity, texColor.a);
}
