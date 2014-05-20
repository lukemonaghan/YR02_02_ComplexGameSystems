#version 330
 
layout(location = 0) in vec4 Position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 TexCoord1;
 
void main()
{
    gl_Position = Projection * View * Model * Position;
    TexCoord1 = Position.xyz;
    TexCoord1.z = -Position.z;
}