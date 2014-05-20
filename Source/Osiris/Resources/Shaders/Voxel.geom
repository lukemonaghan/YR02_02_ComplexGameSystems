#version 330

layout (points) in;
layout (triangle_strip,max_vertices = 24) out;

in vec4 vColour[];
in vec3 vPosition[];
in int vFaces[];
in int vType[];

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 MVP;

uniform float Offset;
uniform int NormalFaces;

out vec4 gColour;
out vec3 gNormal;
out vec2 gTexCoord1;

vec4 vertices[8] = vec4[8](
	vec4(	   0,	   0, Offset, 0.0f),	//A 0
	vec4( Offset,	   0, Offset, 0.0f),	//B 1
	vec4(	   0, Offset, Offset, 0.0f),	//C 2
	vec4( Offset, Offset, Offset, 0.0f),	//D 3
	vec4(	   0,	   0,	   0, 0.0f),	//F 4
	vec4( Offset,	   0,	   0, 0.0f),	//G 5
	vec4(	   0, Offset,	   0, 0.0f),	//E 6
	vec4( Offset, Offset,	   0, 0.0f)		//H 7
	);
int indices[24] = int[24](0, 1, 2, 3, 1, 5, 3, 7, 5, 4, 7, 6, 4, 0, 6, 2, 2, 3, 6, 7, 4, 5, 0, 1);


bool CullFaces(int currentFace, int FaceFlags);
void main(){

	int FaceFlags = vFaces[0] | NormalFaces;
	if (vType[0] == 0 || FaceFlags == 63)
		return;

	gColour = vColour[0];
	vec4 Pos = vec4(vPosition[0],1);

	int CurrentFace = -1;
	for (int i = 0;i < 24;i++){
		int iMod = int(mod(i,4));
		CurrentFace += (iMod == 0) ? 1 : 0;

		if (CullFaces(CurrentFace, FaceFlags)) continue;

		// Get the Appropriate Position
		gl_Position = MVP * (Pos + vertices[indices[i]]);
		EmitVertex();
		if (iMod == 3) EndPrimitive();
	}
}

bool CullFaces(int CurrentFace, int FaceFlags) {
	//	Cull any faces that have been flagged to be culled
	//	flags are set in binary treating the int as a bitset
	switch (CurrentFace) {
		case 0: gNormal = vec3( 0, 0, 1); if ((FaceFlags & 32) == 32) return true; break; // Z+
		case 1: gNormal = vec3( 1, 0, 0); if ((FaceFlags &  8) ==  8) return true; break; // X+
		case 2: gNormal = vec3( 0, 0,-1); if ((FaceFlags &  4) ==  4) return true; break; // Z-
		case 3: gNormal = vec3(-1, 0, 0); if ((FaceFlags &  1) ==  1) return true; break; // X-
		case 4: gNormal = vec3( 0, 1, 0); if ((FaceFlags & 16) == 16) return true; break; // Y+
		case 5: gNormal = vec3( 0,-1, 0); if ((FaceFlags &  2) ==  2) return true; break; // Y-
	}
	return false;
}