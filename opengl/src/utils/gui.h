#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "../utils/logger.h"
#include "../systems/window/window_context.h"
#include "../systems/settings/render_settings.h"

static void ShowLogsWindow() {
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (const std::string& logEntry : Logger::GetHistory())
    {
        ImGui::TextUnformatted(logEntry.c_str());
    }
    ImGui::EndChild();
}

static void ShowTitle(float normalizedLeftOffset, const char* title) {
    float headerWidth = ImGui::GetWindowWidth();
    ImGui::Text("");
    ImGui::SameLine((headerWidth - ImGui::CalcTextSize(title).x) * 0.1f);
    ImGui::Text(title);
    ImGui::Separator();
}


static void ShowDemoWindow(const char* title, std::function<void()> windowContent) {
    ImGui::SetNextWindowPos(ImVec2(20, 20));
    ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    windowContent();
    ImGui::End();
};




