#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D nightTexture;
uniform sampler2D dayTexture;
uniform sampler2D cloudTexture;

void main()
{
    FragColor = mix(texture(dayTexture,TexCoord),texture(cloudTexture,TexCoord),0.2);
}