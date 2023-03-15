#version 330 core

out vec4 FragColor;

in vec3 crntPos;

in vec2 texCoord;

uniform sampler2D diffuse0;

uniform vec4 lightColor;

vec4 ambientLight()
{   
    float ambient = 1.0f;
    return texture(diffuse0, texCoord) * ambient * lightColor;
}

void main()
{
    if (texture(diffuse0, texCoord).a < 0.1)
        discard;

    FragColor = ambientLight();
}
