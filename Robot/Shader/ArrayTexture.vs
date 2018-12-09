//vertex shader
#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform	mat4 proj;
uniform	mat4 view;
uniform mat4 model;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = proj * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
}