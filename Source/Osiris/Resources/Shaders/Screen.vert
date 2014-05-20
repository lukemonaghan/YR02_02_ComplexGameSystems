#version 150

in vec4 Position;
in vec4 Colour;
in vec2 UV;

out vec4 vColour;
out vec2 vUV;

uniform mat4 Model 		= mat4(1);
uniform mat4 View  		= mat4(1);
uniform mat4 Projection = mat4(1);

void main(){
	vColour 	= Colour;
	vUV 		= UV;
	gl_Position = Projection * View * Model * Position;
}

