//FBO fragment shader
#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture1;
uniform sampler2D screenTexture2;

void main()
{
	if(TexCoords.x>=0.5)
		FragColor = vec4(texture(screenTexture2, TexCoords.st).rgb,1.0);
	else
		FragColor = vec4(texture(screenTexture1, TexCoords.st).rgb,1.0);
} 