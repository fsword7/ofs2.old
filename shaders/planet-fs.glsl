#version 400

layout (binding=0) in sampler2D sTexture;

in vec2 texCoord;
in vec3 myColor;

out vec4 fragColor;

int main()
{
    fragColor = texture(sTexture, texCoord);
}