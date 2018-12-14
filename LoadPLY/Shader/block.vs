//vertex shader
#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 proj;
uniform mat4 view;

out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = proj * view * vec4(aPos.x, aPos.y, 0.0, 1.0);
}