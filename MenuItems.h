#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool OpenProjectSettings = false;

void popupProjectSettings()
{

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Appearing);
	ImGui::Begin("Project Settings");
	if (ImGui::Button("Close")) {
		OpenProjectSettings = false;
	}
	ImGui::End();
}