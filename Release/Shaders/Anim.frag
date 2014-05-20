#version 330

in vec2 vUV;
in vec4 vColor;

//-------------------------------------

// output color
out vec4 outColor;

// textures
uniform sampler2D DiffuseTexture;

void main() 
{ 
	outColor = texture2D( DiffuseTexture, vUV.xy ) * vColor;
	outColor.a = 1;
}