#ifndef DILIGHT_CLASS_H
#define DILIGHT_CLASS_H
#include<glm/glm.hpp>
class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();
	glm::vec3 getRawLightPosition();
	float getX() { return X; }
	float getY() { return Y; }
	void calculateLightRawToCoords();
	void shaderSetup(Shader shader);
	void calculateShadowsProj(Camera3D camera3D);
	void viewport();
	glm::mat4 getLightView() { return lightView; }
	bool shadows() { return renderShadows; }
	glm::mat4 getLightProjection() { return lightProjection; }
	float X;
	float Y;

	bool renderShadows = true;
	int shadowMapWidth = 3000;
	int shadowMapHeight = 3000;
	float camShadowDistance = 500.0f;

private:
	glm::mat4 lightView;
	glm::mat4 orthgonalProjection;
	glm::mat4 lightProjection;
	glm::vec3 lightRawPos = glm::vec3(0.5f, 0.8f, 0.5f);

};
void DirectionalLight::viewport() {
	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
}
void DirectionalLight::calculateShadowsProj(Camera3D camera3D)
{
	lightView = glm::lookAt(20.0f * getRawLightPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightProjection = orthgonalProjection * lightView;
		orthgonalProjection = glm::ortho(
			-camShadowDistance + camera3D.Position.x,
			camShadowDistance + camera3D.Position.x,
			-camShadowDistance + camera3D.Position.y,
			camShadowDistance + camera3D.Position.y,
			-camShadowDistance + camera3D.Position.z,
			camShadowDistance + camera3D.Position.z
	);
}
glm::vec3 DirectionalLight::getRawLightPosition()
{
	return lightRawPos;
}
void DirectionalLight::calculateLightRawToCoords()
{
	float rotationSpeed = 0.1f;
	float adjustedLightX = X * rotationSpeed;
	float adjustedLightY = Y * rotationSpeed;
	float radius = 5.0f;
	lightRawPos.x = radius * cos(adjustedLightX); 
	lightRawPos.z = radius * sin(adjustedLightX);
	lightRawPos.y = adjustedLightY; 

}
void DirectionalLight::shaderSetup(Shader shader)
{
	
	//shadowFramebuffer = new Framebuffer(shadowMapWidth, shadowMapHeight, true); // 'true' for depth-only

	orthgonalProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.01f, 300.000f);
	lightView = glm::lookAt(20.0f * getRawLightPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightProjection = orthgonalProjection * lightView;


	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
}
DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}
#endif