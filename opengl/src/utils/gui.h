#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "../utils/logger.h"
#include "../systems/window/window_context.h"
#include "../systems/settings/render_settings.h"

static void ShowLogsWindow() {
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (const std::string& logEntry : Logger::GetHistory())
    {
        ImGui::TextUnformatted(logEntry.c_str());
    }
    ImGui::EndChild();
}

static void ShowTitle(float normalizedLeftOffset, const char* title) {
    float headerWidth = ImGui::GetWindowWidth();
    ImGui::Text("");
    ImGui::SameLine((headerWidth - ImGui::CalcTextSize(title).x) * normalizedLeftOffset);
    ImGui::Text(title);
    ImGui::Separator();
}


static void ShowDemoGUI(const char* title, const std::function<void()>& windowContent) {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 640, 40));
    ImGui::SetNextWindowSize(ImVec2(600, 0));
    
    ImGui::Begin(title, nullptr);
    windowContent();
    ImGui::End();
};




