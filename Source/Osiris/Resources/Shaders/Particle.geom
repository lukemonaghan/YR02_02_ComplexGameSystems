#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 vPosition[];
in vec4 vColour[];

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Camera;
uniform vec3 BillboardAxis = vec3(1,1,1);

out vec4 gColour;

void main()
{
	gColour = vColour[0];

	vec3 corners[4];
	corners[0] = vec3(-0.5f,-0.5f, 0);
	corners[1] = vec3( 0.5f,-0.5f, 0);
	corners[2] = vec3(-0.5f, 0.5f, 0);
	corners[3] = vec3( 0.5f, 0.5f, 0);

	// billboard
	vec3 zAxis = BillboardAxis.z == 1 ? normalize( Camera[3].xyz - vPosition[0].xyz ) : vec3(0,0,1);
	vec3 xAxis = BillboardAxis.x == 1 ? cross( Camera[1].xyz, zAxis ) : vec3(1,0,0);
	vec3 yAxis = BillboardAxis.y == 1 ? cross( zAxis, xAxis ) : vec3(0,1,0);
	mat3 billboard = mat3(xAxis,yAxis,zAxis);

	mat4 MVP = Projection * View * Model;

	for (int i = 0; i < 4; i++) {
		gl_Position = MVP * vec4(billboard * corners[i] + vPosition[0].xyz, 1);
		EmitVertex();
	}
}