#version 330

in vec4 vPosition;
in vec2 vTexCoord1;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D WorldTexture;
uniform sampler2D DepthTexture;
uniform int DrawMode = 1;

uniform float Near = 0.1f;
uniform float Far = 50.0f;

out vec4 fColour;

float LinearDepth(float DepthBufferZ) {
	return (2.0 * Near) / (Far + Near - DepthBufferZ * (Far - Near));
}

void main() {

	vec4 DiffuseColour = texture(DiffuseTexture, vTexCoord1);
	vec4 NormalColour = texture(NormalTexture, vTexCoord1);
	vec4 WorldColour = texture(WorldTexture, vTexCoord1);
	vec4 DepthColour = vec4(LinearDepth(texture( DepthTexture, vTexCoord1 ).r));

	switch (DrawMode) {
		case 1: fColour = DiffuseColour; break;
		case 2: fColour = NormalColour; break;
		case 3: fColour = WorldColour; break;
		case 0: fColour = DepthColour; break;
	}
}