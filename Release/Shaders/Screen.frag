#version 150

in vec4 vColour;
in vec2 vUV;

out vec4 outColour;

uniform sampler2D ElementTexture;

void main(){
	outColour = texture (ElementTexture,vUV) * vColour;
	if (outColour.a < 0.05f) { discard; }//remove alpha less than 0.05f
}

