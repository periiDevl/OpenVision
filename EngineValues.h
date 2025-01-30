#ifndef ENGINE_VALUES_CLASS_H
#define ENGINE_VALUES_CLASS_H
#include"Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
class Values
{
public:
	Values();
	~Values();
	void antiAliasingValues(Shader shader);
	void antiAliasingUI(Shader shader);


	float minEdgeContrast = 128.0f;
	float subPixelAliasing = 8.0f;
	float maximumEdgeDetection = 128.0f;


	void VignetteValues(Shader shader);
	void VignetteUI(Shader shader);
	float vignetteRadius = 1.0f;
	float vignetteSoftness = 1.0f;

	void GammaAndExposureValues(Shader shader);
	void GammaAndExposureUI(Shader shader);
	float gamma = 1.6f;
	float exposure = 1.0f;


	void ShadowValues(Shader shader);
	void ShadowUI(Shader shader, DirectionalLight& dLight);
	float avgShadow = 1.0f;
	float bias1 = 0.005f;
	float bias2 = 0.0005f;
	int sampleRadius = 2;


private:

};
//Shadow
void Values::ShadowValues(Shader shader)
{
	shader.activate();
	shader.setFloat("avgShadow", avgShadow);
	shader.setFloat("bias1", bias1);
	shader.setFloat("bias2", bias2);
	shader.setInt("sampleRadius", sampleRadius);

}
void Values::ShadowUI(Shader shader, DirectionalLight& dLight)
{
	ShadowValues(shader);
	ImGui::Separator();
	ImGui::SetNextItemWidth(80.0f);
	ImGui::InputFloat("i:", &dLight.camShadowDistance);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(300.0f);
	ImGui::SliderFloat("Camera Shadow Draw Distance", &dLight.camShadowDistance, 300, 5000);

	ImGui::SetNextItemWidth(80.0f);
	ImGui::InputFloat("AvgShadow", &avgShadow);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(140.0f);
	ImGui::SliderFloat("Avg Shadow Ambiance", &avgShadow, 0, 1);
	ImGui::InputFloat("Bias 1", &bias1);
	ImGui::InputFloat("Bias 2", &bias2);
	ImGui::InputInt("Sample Radius", &sampleRadius);

	ImGui::BeginGroup();
	ImGui::Text("Render Shadows (disable FrameBuffer)");
	ImGui::SameLine();
	ImGui::Checkbox("", &dLight.renderShadows);
	ImGui::SetNextItemWidth(60.0f);
	ImGui::InputInt("ShadowMap X", &dLight.shadowMapWidth, 0);
	ImGui::SetNextItemWidth(60.0f);
	ImGui::InputInt("ShadowMap Y", &dLight.shadowMapHeight, 0);

}

//Gamma Exposure
void Values::GammaAndExposureValues(Shader shader) {
	shader.activate();
	shader.setFloat("gamma", gamma);
	shader.setFloat("exposure", exposure);
}
void Values::GammaAndExposureUI(Shader shader) {
	GammaAndExposureValues(shader);
	ImGui::Separator();
	ImGui::SetNextItemWidth(80.0f);
	ImGui::InputFloat("Gamma", &gamma);
	ImGui::SetNextItemWidth(80.0f);
	ImGui::InputFloat("Exposure", &exposure);
}
//AntiAliasing
void Values::antiAliasingValues(Shader shader) {
	shader.activate();
	shader.setFloat("minEdgeContrast", minEdgeContrast);
	shader.setFloat("subPixelAliasing", subPixelAliasing);
	shader.setFloat("maximumEdgeDetection", maximumEdgeDetection);
}
void Values::antiAliasingUI(Shader shader) {
	antiAliasingValues(shader);
	ImGui::Separator();
	ImGui::Text("Anti-Aliasing(FXAA) :");
	ImGui::InputFloat("MinEdgeContrast", &minEdgeContrast);
	ImGui::InputFloat("SubPixelAliasing", &subPixelAliasing);
	ImGui::InputFloat("MaximumEdgeDetection", &maximumEdgeDetection);


}
//bloom
void Values::VignetteValues(Shader shader)
{
	shader.activate();
	shader.setFloat("radius", vignetteRadius);
	shader.setFloat("softness",vignetteSoftness );

}
void Values::VignetteUI(Shader shader)
{
	VignetteValues(shader);
	ImGui::Separator();
	ImGui::Text("Vignette :");
	ImGui::InputFloat("Vignette-Radius", &vignetteRadius);
	ImGui::InputFloat("Vignette-Softness", &vignetteSoftness);

}

Values::Values()
{
}

Values::~Values()
{
}
#endif