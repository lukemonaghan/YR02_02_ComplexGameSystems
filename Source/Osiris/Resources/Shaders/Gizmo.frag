#version 330

in vec4 vPosition;
in vec4 vColour;
in vec2 vTexCoord1;

uniform sampler2D DiffuseTexture;

out vec4 fColour;

void main() {
	//vec4 Diffuse = texture(DiffuseTexture, vTexCoord1);
	//fColour = Diffuse * vColour;
	fColour = vColour;
}

