#version 330

layout (location = 0) in vec4 Colour;
layout (location = 1) in vec3 Position;
layout (location = 2) in int Faces;
layout (location = 3) in int Type;

out vec4 vColour;
out vec3 vPosition;
out int vFaces;
out int vType;

void main() {
	vColour = Colour;
	vPosition = Position;
	vFaces = Faces;
	vType = Type;
}