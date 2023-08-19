#pragma once
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#define MY_THEME_COLOR_PRIMARY      ImVec4(0.2f, 0.2f, 0.2f, 1.0f)
#define MY_THEME_COLOR_SECONDARY    ImVec4(0.15f, 0.15f, 0.15f, 1.0f)
#define MY_THEME_COLOR_TEXT         ImVec4(0.8f, 0.8f, 0.8f, 1.0f)
#define MY_THEME_COLOR_WINDOW_BG    ImVec4(0.1f, 0.1f, 0.1f, 1.0f)
#define MY_THEME_COLOR_BORDER       ImVec4(0.3f, 0.3f, 0.3f, 0.5f)
#define MY_THEME_COLOR_HEADER       ImVec4(0.15f, 0.15f, 0.15f, 1.0f)
#define MY_THEME_COLOR_COLLAPSED    ImVec4(0.3f, 0.3f, 0.3f, 0.6f)
#define MY_THEME_COLOR_HOVER        ImVec4(0.25f, 0.25f, 0.25f, 1.0f)

void DefaultTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Modify ImGui style settings
    style.WindowPadding = ImVec2(10, 10);
    style.WindowRounding = 4.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.FrameBorderSize = 1.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);



    colors[ImGuiCol_Text] = MY_THEME_COLOR_TEXT;
    colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    colors[ImGuiCol_WindowBg] = MY_THEME_COLOR_WINDOW_BG;
    colors[ImGuiCol_Border] = MY_THEME_COLOR_BORDER;
    colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_FrameBg] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_FrameBgHovered] = MY_THEME_COLOR_HOVER;
    colors[ImGuiCol_FrameBgActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_TitleBg] = MY_THEME_COLOR_HEADER;
    colors[ImGuiCol_TitleBgActive] = MY_THEME_COLOR_HEADER;
    colors[ImGuiCol_TitleBgCollapsed] = MY_THEME_COLOR_HEADER;
    colors[ImGuiCol_MenuBarBg] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_ScrollbarBg] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_ScrollbarGrab] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_ScrollbarGrabHovered] = MY_THEME_COLOR_HOVER;
    colors[ImGuiCol_ScrollbarGrabActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_CheckMark] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_SliderGrab] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_SliderGrabActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_Button] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_ButtonHovered] = MY_THEME_COLOR_HOVER;
    colors[ImGuiCol_ButtonActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_Header] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_HeaderHovered] = MY_THEME_COLOR_HOVER;
    colors[ImGuiCol_HeaderActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_ResizeGrip] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_ResizeGripHovered] = MY_THEME_COLOR_HOVER;
    colors[ImGuiCol_ResizeGripActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_PlotLines] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_PlotLinesHovered] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_PlotHistogram] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_PlotHistogramHovered] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_TextSelectedBg] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_DragDropTarget] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_NavHighlight] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_NavWindowingHighlight] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_NavWindowingDimBg] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_ModalWindowDimBg] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_Tab] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_TabHovered] = MY_THEME_COLOR_HOVER;
    colors[ImGuiCol_TabActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_TabUnfocused] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_TabUnfocusedActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_Tab] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_TabHovered] = MY_THEME_COLOR_HOVER;
    colors[ImGuiCol_TabActive] = MY_THEME_COLOR_PRIMARY;
    colors[ImGuiCol_TabUnfocused] = MY_THEME_COLOR_SECONDARY;
    colors[ImGuiCol_TabUnfocusedActive] = MY_THEME_COLOR_PRIMARY;

    style.ChildRounding = 4.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 4.0f;
    style.PopupBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
}

