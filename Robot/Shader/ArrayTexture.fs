//fragment shader
#version 430 core

out vec4 Fcolor;

in vec2 TexCoords;

uniform sampler2DArray Array_tex;
uniform int SpriteIndex;

void main()
{
	vec4 col = texture(Array_tex, vec3(TexCoords, SpriteIndex));
	if(col.a < 0.1)
        discard;
	Fcolor = col;
}