#version 330

in vec4 gColour;
in vec3 gNormal;
in vec2 gTexCoord1;

uniform vec3 LightDir = vec3(-1);
uniform vec3 LightCol = vec3(1);
uniform vec3 Ambient = vec3(0.5f);

out vec4 fColour;

vec3 Light();
void main() {
	// Colour
	//fColour = gColour;

	fColour = gColour * vec4(Light(),1);
}

vec3 Light() {
	vec3 N = normalize(gNormal);
	vec3 L = normalize(LightDir) * -1.0f;

	float lambert = max(0, dot(N,L));

	return Ambient + (LightCol * lambert);
}