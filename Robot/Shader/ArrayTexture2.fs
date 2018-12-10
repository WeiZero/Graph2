//fragment shader
#version 430 core

out vec4 Fcolor;

in vec2 TexCoords;

uniform sampler2DArray TexArray;
uniform int SpriteIndex = 0;

void main()
{
	
	//vec4 col = vec4(1,0,SpriteIndex,1);
	vec4 col = texture(TexArray, vec3(TexCoords, SpriteIndex));
	Fcolor = col;
}