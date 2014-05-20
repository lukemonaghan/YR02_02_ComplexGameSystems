#version 330

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Colour;
layout(location = 7) in vec2 TexCoord1;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 vColour;
out vec2 vTexCoord1;

void main() {
	gl_Position = (Projection * View * Model) * Position;
	vColour = Colour;
	vTexCoord1 = TexCoord1;
}