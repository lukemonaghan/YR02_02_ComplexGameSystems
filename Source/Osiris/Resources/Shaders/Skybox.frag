#version 330
 
in vec3 TexCoord1;

uniform samplerCube CubeMapTexture;

out vec4 fColour;

 
void main (void) 
{
    fColour = texture(CubeMapTexture, TexCoord1);
}