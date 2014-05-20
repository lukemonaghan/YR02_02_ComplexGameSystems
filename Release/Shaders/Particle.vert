#version 330

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Colour;

out vec4 vPosition;
out vec4 vColour;

void main()
{
	vPosition = Position;
	vColour = Colour;
}