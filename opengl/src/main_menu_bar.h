#pragma once
#include "scenes\scene_selection.h"
#include "imgui/imgui.h"
#include "systems/settings/render_settings.h"

class MainMenuBar {
public:
    MainMenuBar(float spacing,  RenderSettings& settings, SceneSelection& demoSelection) :
    spacing(spacing),
    m_settings(settings),
    m_demoSelection(demoSelection) {}

    void Show(bool &UI_ENABLED);

private:
    float spacing;
    RenderSettings& m_settings;
    SceneSelection& m_demoSelection;
};

inline void MainMenuBar::Show(bool& UI_ENABLED) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(RenderSettings::INFO_HEADER)) {
            m_settings.ShowInfoChild();
            ImGui::EndMenu();
        }

        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.x * spacing);

        if (ImGui::BeginMenu(RenderSettings::SETTINGS_HEADER)) {
            m_settings.ShowSettingsChild();
            ImGui::EndMenu();
        }

        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.x * spacing);

        if (ImGui::BeginMenu(SceneSelection::SELECTION_HEADER)) {
            m_demoSelection.ShowSelectionChild();
            ShowTitle(0.1, ICON_FK_COG " Options");
            ImGui::MenuItem("Display UI", "", &UI_ENABLED);
            ImGui::EndMenu();
        }

        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.x * spacing);

        if (ImGui::BeginMenu(RenderSettings::LOGS_HEADER)) {
            m_settings.ShowLogsChild();
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
