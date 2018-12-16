//FBO fragment shader
#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float dead;

void main()
{
	if(dead==0)
		FragColor = texture(screenTexture, TexCoords);
	else
	{	
		vec4 texture_color = texture(screenTexture, TexCoords);							
		float grayscale_color = 0.299*texture_color.r + 0.587*texture_color.g + 0.114*texture_color.b;
		FragColor = vec4(grayscale_color, grayscale_color, grayscale_color, 1.0);
	}
} 