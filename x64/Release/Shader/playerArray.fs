//fragment shader
#version 430 core

out vec4 Fcolor;

in vec2 TexCoords;

uniform sampler2DArray Array_tex;
uniform int SpriteIndex;
uniform int dir;

void main()
{
	vec4 col = texture(Array_tex, vec3(TexCoords.x,TexCoords.y, SpriteIndex));
	if(dir == 0)
		col = texture(Array_tex, vec3(1 - TexCoords.x,TexCoords.y, SpriteIndex));
	if(col.a< 0.1)
			discard;
	Fcolor = col;
}