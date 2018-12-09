//fragment shader
#version 430 core

out vec4 Fcolor;

in vec2 TexCoords;

uniform sampler2DArray TexArray;
uniform int SpriteIndex;

void main()
{
	vec4 col = texture(TexArray, vec3(TexCoords, SpriteIndex));
	Fcolor = col;
}