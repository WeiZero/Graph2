//fragment shader
#version 430 core

out vec4 Fcolor;

in vec2 TexCoords;

uniform sampler2D Tex;

void main()
{
	vec4 col = texture(Tex, TexCoords);
	if(col.a< 0.1)
		discard;
	Fcolor = col;
}