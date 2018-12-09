//fragment shader
#version 430 core

out vec4 Fcolor;

in vec2 TexCoords;

uniform sampler2D TextureM;

void main()
{
	vec4 col = texture(TextureM, TexCoords);
    Fcolor = col;
}