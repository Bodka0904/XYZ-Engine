#type vertex;
#version 430 core

layout(std140, binding = 0) uniform XYZ_OrthoCamera
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec2 ViewPos;
};