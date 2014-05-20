#version 330

layout (location = 0) in vec4 Position;
layout (location = 7) in vec2 TexCoord1;

out vec4 vPosition;
out vec2 vTexCoord1;

void main() {
	vTexCoord1 = TexCoord1;
	vPosition = Position;
	gl_Position = vPosition;
}