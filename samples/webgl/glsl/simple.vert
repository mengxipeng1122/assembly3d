uniform mat4 Projection;
uniform mat4 ModelView;
uniform mat4 Lighting;

attribute vec3 Position;
attribute vec3 Normal;
attribute vec2 TexCoord;

varying vec3 _Normal;
varying vec2 _TexCoord;

void main(void) 
{
		gl_Position = Projection * ModelView * vec4(Position, 1.0);
    _TexCoord = TexCoord;
		_Normal = (Lighting * vec4(Normal, 0.0)).xyz;
}
