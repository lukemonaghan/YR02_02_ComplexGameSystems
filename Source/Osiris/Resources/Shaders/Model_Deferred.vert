#version 330

layout(location = 0) in vec4 Position;
layout(location = 2) in vec4 Normal;
layout(location = 7) in vec2 TexCoord1;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 vNormal;
out vec2 vTexCoord1;
out vec4 vWorldPos;

void main() {
	vNormal = Normal;
	vTexCoord1 = TexCoord1;

	vWorldPos = Model * Position;
	
	gl_Position = Projection * View * Model * Position;
}  