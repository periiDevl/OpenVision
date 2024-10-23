#ifndef OV_H
#define OV_H
#include <fstream>
#include <iostream>
#include <filesystem>
#include<thread>
#include"Console.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <imgui.h> 
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class OV {

public:



	static void renderText(const std::string& text, const std::string& fontPath, float x, float y, float fontSize = 14.0f, ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) {
		static std::unordered_map<std::string, ImFont*> loadedFonts;
		static std::unordered_map<std::string, GLuint> fontTextures;

		ImGuiIO& io = ImGui::GetIO();

		ImFont* font = nullptr;
		auto fontIter = loadedFonts.find(fontPath);
		if (fontIter != loadedFonts.end()) {

			font = fontIter->second;
		}
		else {

			font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize);
			if (!font) {
				std::cerr << "Failed to load font: " << fontPath << std::endl;
				return;
			}
			loadedFonts[fontPath] = font;
		}

		GLuint fontTexture = 0;
		auto textureIter = fontTextures.find(fontPath);
		if (textureIter != fontTextures.end()) {

			fontTexture = textureIter->second;
		}
		else {

			unsigned char* pixels;
			int width, height;
			io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

			glGenTextures(1, &fontTexture);
			glBindTexture(GL_TEXTURE_2D, fontTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			fontTextures[fontPath] = fontTexture;
		}

		io.Fonts->TexID = reinterpret_cast<void*>(static_cast<intptr_t>(fontTexture));

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 prevWindowBg = style.Colors[ImGuiCol_WindowBg];
		ImVec4 prevChildBg = style.Colors[ImGuiCol_ChildBg];
		float prevAlpha = style.Colors[ImGuiCol_WindowBg].w;

		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

		ImGui::PushFont(font);

		ImGui::SetNextWindowPos(ImVec2(x, y));
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Text, textColor);

		ImGui::Begin("Text", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBackground);

		ImGui::Text("%s", text.c_str());

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopFont();
		style.Colors[ImGuiCol_WindowBg] = prevWindowBg;
		style.Colors[ImGuiCol_ChildBg] = prevChildBg;
		style.Colors[ImGuiCol_WindowBg].w = prevAlpha;

	}



private:
	/*
	static void playSoundThread(const std::string& filePath)
	{
		sf::SoundBuffer buffer;
	
		if (!buffer.loadFromFile(filePath))
		{
			return;
		}
	
		sf::Sound sound;
		sound.setBuffer(buffer);
		sound.play();
	
		while (sound.getStatus() == sf::Sound::Playing)
		{
		}
	
	}
	static void playSoundThreadLooped(const std::string& filePath)
	{
		sf::SoundBuffer buffer;
	
		if (!buffer.loadFromFile(filePath))
		{
			return;
		}
	
		sf::Sound sound;
		sound.setBuffer(buffer);
		sound.setLoop(true); // Set the sound to loop
		sound.play();
	
		while (sound.getStatus() == sf::Sound::Playing)
		{
		}
	}
	*/
};

#endif
