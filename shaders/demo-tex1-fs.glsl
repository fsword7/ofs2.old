#version 330

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fragColor = mix(texture(texture1, texCoords), texture(texture2, texCoords), 0.2);
}