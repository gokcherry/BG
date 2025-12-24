VertexShader
#version 330 core 
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoord;
out vec2 textureCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uWorldMatrix;
void main()
{
   gl_Position = uProjectionMatrix*uWorldMatrix*vec4(inPosition.x,inPosition.y,inPosition.z, 1.0);
   textureCoord=inTextureCoord;
}                         