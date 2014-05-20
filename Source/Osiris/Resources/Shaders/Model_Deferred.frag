#version 330

in vec4 vNormal;
in vec2 vTexCoord1;

in vec4 vWorldPos;

out vec4 oDiffuse;
out vec4 oNormal;
out vec4 oWorld;

uniform sampler2D DiffuseTexture;

void main()
{
    // Just pass the data directly in
	oDiffuse = texture(DiffuseTexture, vTexCoord1);
	oNormal = vNormal;
	oWorld = vWorldPos;
}