#ifndef SETTINGS_H
#define SETTINGS_H


//Global settings
float GlobalWorldScale = 0.5f;

//const unsigned int width = 1920;
//const unsigned int height = 1080;

//Technical sttings
Vertex vertices[] =
{
	Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, -1.0f)},
	Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, -1.0f)},
	Vertex{glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};
std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));


const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aPos;

layout(location = 1) in vec3 aNormal;

layout(location = 2) in vec3 aColor;

layout(location = 3) in vec2 aTex;

out vec3 crntPos;

out vec3 Normal;

out vec3 color;

out vec2 texCoord;

uniform mat4 camMatrix;

uniform mat4 model;

void main()
{
    crntPos = vec3(model * vec4(aPos, 1.0f));
    Normal = aNormal;
    color = aColor;
    texCoord = aTex;
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}
)";
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D diffuse0;
uniform float tileX = 2.0f;
uniform float tileY = 2.0f;
vec4 lightColor = vec4(1, 1, 1, 1);
uniform vec4 tint;
uniform float cornerRadius = 0.1;
vec4 roundedRectangle(vec2 p, vec2 size, float radius)
{
    float aspectRatio = size.x / size.y;
    
    float scaledRadius = radius * min(aspectRatio, 1.0);
    
    vec2 halfSize = size * 0.5;
    vec2 position = abs(p - halfSize) - halfSize + vec2(scaledRadius);
    float length = length(max(position, 0.0)) - scaledRadius;
    float inside = length < 0.0 ? 1.0 : 0.0;

    float alpha = inside == 1.0 ? 1.0 : 0.0;

    return vec4(inside, inside, inside, alpha);
}

void main()
{
    
    

    vec2 tiledTexCoord = fract(texCoord * vec2(tileX, tileY));
    vec4 texColor = texture(diffuse0, tiledTexCoord);

    float ambient = 1.0f;
    vec4 bgColor = vec4(0.0);

    vec2 roundedRectSize = vec2(1.0, 1.0);
    

    vec4 mask = roundedRectangle(tiledTexCoord, roundedRectSize, cornerRadius);

    if (mask.a < 0.5) {
        discard;
    }
    if (texColor.a < 0.5)
        discard;
    FragColor = mix(bgColor, texColor, texColor.a) * ambient * lightColor * tint * mask;
}
)";


const char* UnlitFragmentFire =
R"(
#version 330 core

out vec4 FragColor;

vec3 fire(float value) {
  float numRepeats = 2.0;
  value = fract(value * numRepeats);

  float half = 0.5;
  if (value < half) {
    return mix(vec3(1.0, 0.0, 0.0), vec3(1.0, 0.7, 0.0), value * 2.0);
  } else {
    return mix(vec3(1.0, 0.7, 0.0), vec3(1.0, 0.0, 0.0), (value - half) * 2.0);
  }
}

void main() {
  vec2 fragCoord = gl_FragCoord.xy / vec2(1920.0 * 4.0, 1080.0 * 4.0);

  float scaledValue = fract(fragCoord.x * 5.0);

  vec3 fireColor = fire(scaledValue);

  FragColor = vec4(fireColor, 1.0);
}

)";



const char* unlitFrag =
R"(
#version 330 core

out vec4 FragColor;

uniform vec3 color; // The color of the object

void main()
{
    FragColor = vec4(color, 1.0); // Set the fragment color to the object color
}

)";




const char* FrameBufferFrag = R"(
#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float gamma = 1.6f;

void main()
{
    vec3 fragment = texture(screenTexture, texCoords).rgb;
    vec3 bloom = texture(bloomTexture, texCoords).rgb;

    vec3 color = fragment + bloom;

    float exposure = 4.0f;
    vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);

    FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
}
)";


const char* FrameBufferVert =
R"(
#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0); 
    texCoords = inTexCoords;
}  
)";



float rectangleVertices[] =
{
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f
};
#endif // !SETTINGS_H