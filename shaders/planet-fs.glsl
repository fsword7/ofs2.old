#version 400

//layout (binding=0) in sampler2D sTexture;

in vec2 texCoord;
in vec4 myColor;

out vec4 fragColor;

void main()
{
//   fragColor = texture(sTexture, texCoord);
    fragColor = myColor;
}