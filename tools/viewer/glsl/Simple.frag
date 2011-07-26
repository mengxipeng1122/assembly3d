//#version 110

uniform sampler2D firstTexture; 

//in
varying vec3 fragmentNormal;
varying vec2 fragmentTexCoord;


// out 
// out vec4 fragColor

void main(void)
{
	float intensity;
	
	//intensity = max(dot(fragmentNormal, vec3(0.0, 0.0, 1.0)), 0.0);
//vec4(intensity,intensity,intensity,1.0);//
	gl_FragColor = texture2D(firstTexture, fragmentTexCoord);//*intensity;
}
