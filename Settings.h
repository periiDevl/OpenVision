#pragma once
//Global settings
float GlobalWorldScale = 0.5f;


//Window settings
glm::vec2 rattio(16, 9);

const unsigned int width = 95 * rattio.x;
const unsigned int height = 95 * rattio.y;


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

const char* vertexShaderSource = 
"#version 330 core \n"

"layout(location = 0) in vec3 aPos;\n"

"layout(location = 1) in vec3 aNormal;\n"

"layout(location = 2) in vec3 aColor;\n"

"layout(location = 3) in vec2 aTex;\n"

"out vec3 crntPos;\n"

"out vec3 Normal;\n"

"out vec3 color;\n"

"out vec2 texCoord;\n"

"uniform mat4 camMatrix;\n"

"uniform mat4 model;\n"

"void main()\n"
"{ \n"
"	crntPos = vec3(model * vec4(aPos, 1.0f));\n"
"	Normal = aNormal;\n"
"	color = aColor;\n"
"	texCoord = aTex;\n"
"	gl_Position = camMatrix * vec4(crntPos, 1.0);\n"
"}\n";


const char* fragmentShaderSource =
"#version 330 core \n"

"out vec4 FragColor;\n"

"in vec3 crntPos;\n"

"in vec2 texCoord;\n"

"uniform sampler2D diffuse0;\n"

"vec4 lightColor = vec4(1, 1, 1, 1);\n"

"vec4 ambientLight()\n"
"{ \n"
"	float ambient = 1.0f;\n"
"	return texture(diffuse0, texCoord) * ambient * lightColor;\n"
"}\n"

"void main()\n"
"{ \n"
"	FragColor = ambientLight();\n"
"	if (texture(diffuse0, texCoord).a < 0.01)\n"
"		discard;\n"

"}\n";


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

void main()
{
    vec2 center = vec2(0.5, 0.5);
    //float radius = 0.7;
    //float softness = 0.2;

    float dist = distance(texCoords, center);
    float vignette = smoothstep(radius, radius - softness, dist);

    vec4 color = texture(screenTexture, texCoords);
    FragColor = color * vignette;
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