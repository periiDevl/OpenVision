
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

const char* fragmentShaderSource =
R"(
#version 330 core
out vec4 FragColor;
in vec3 crntPos;
in vec2 texCoord;
uniform sampler2D diffuse0;
uniform float tileX = 2.0f;
uniform float tileY = 2.0f;
vec4 lightColor = vec4(1, 1, 1, 1);

uniform vec4 tint;

vec4 ambientLight()
{
    vec2 tiledTexCoord = fract(texCoord * vec2(tileX, tileY));
    vec4 texColor = texture(diffuse0, tiledTexCoord);


    float ambient = 1.0f;
    vec4 bgColor = vec4(0.0);
    float borderThreshold = 0.001;

    float distanceToBorder = min(min(tiledTexCoord.x, 1.0 - tiledTexCoord.x), min(tiledTexCoord.y, 1.0 - tiledTexCoord.y));

    if (distanceToBorder < borderThreshold)
        discard;
    if (texColor.a < 0.5)
        discard;

    return mix(bgColor, texColor, texColor.a) * ambient * lightColor;
}

void main()
{
    FragColor = ambientLight() * tint;
})";

const char* UnlitFragment =
R"(
#version 330 core
out vec4 FragColor;
uniform vec4 color;
void main()
{
   FragColor = color;
}
)";



const char* FrameBufferFrag = R"(
#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float radius= 0.2;
uniform float softness= 0.2;

uniform float minEdgeContrast;
uniform float subPixelAliasing;
uniform float maximumEdgeDetection;

float FXAA_SPAN_MAX = maximumEdgeDetection;
float FXAA_REDUCE_MIN = 1.0/ minEdgeContrast;
float FXAA_REDUCE_MUL = 1.0/subPixelAliasing;
uniform vec2 resolution;
vec3 ApplyFXAA() {
    vec2 inverse_resolution = vec2(1.0 / resolution.x, 1.0 / resolution.y);
    vec3 rgbNW = texture(screenTexture, texCoords + (vec2(-1.0, -1.0) * inverse_resolution)).rgb;
    vec3 rgbNE = texture(screenTexture, texCoords + (vec2(1.0, -1.0) * inverse_resolution)).rgb;
    vec3 rgbSW = texture(screenTexture, texCoords + (vec2(-1.0, 1.0) * inverse_resolution)).rgb;
    vec3 rgbSE = texture(screenTexture, texCoords + (vec2(1.0, 1.0) * inverse_resolution)).rgb;
    vec3 rgbM = texture(screenTexture, texCoords).rgb;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * 0.25 * FXAA_REDUCE_MUL, FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * inverse_resolution;
    vec3 rgbA = 0.5 * (
        texture(screenTexture, texCoords.xy + dir * (1.0 / 3.0 - 0.5)).rgb +
        texture(screenTexture, texCoords.xy + dir * (2.0 / 3.0 - 0.5)).rgb);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(screenTexture, texCoords.xy + dir * (-0.5)).rgb +
        texture(screenTexture, texCoords.xy + dir * (0.5)).rgb);
    return vec3(rgbB);
}
void main()
{
    vec2 center = vec2(0.5, 0.5);

    float dist = distance(texCoords, center);
    float vignette = smoothstep(radius, radius - softness, dist);
    
    vec3 fxaa = ApplyFXAA();
    
    vec4 color = texture(screenTexture, texCoords);
    
    vec3 finalColor = color.rgb * vignette;
    finalColor = mix(finalColor, fxaa, vignette);
    
    FragColor = vec4(finalColor, 1.0);
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

