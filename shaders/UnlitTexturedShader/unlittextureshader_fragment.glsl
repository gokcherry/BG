FragmentShader
#version 330 core
out vec4 fragColor;
in vec2 textureCoord;   

uniform sampler2D mySampler;
void main()
{                     
   fragColor = texture(mySampler,textureCoord);
}                     