#version 330

in vec4 vColour;
in vec2 vTexCoord1;

uniform sampler2D DiffuseTexture;

out vec4 fColour;

void main() {
	fColour.rgb = texture(DiffuseTexture, vTexCoord1).rrr;
	fColour.a = 1;
}