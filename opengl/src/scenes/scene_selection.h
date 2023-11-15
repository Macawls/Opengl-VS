#pragma once
#include "scene_base.h"
#include "../resources/fonts/forkawesome-icons.h"



// Handles demo selection and switching
class SceneSelection {
public:
    inline static const char* SELECTION_HEADER = ICON_FK_WINDOW_MAXIMIZE " Scenes";
    SceneSelection() = default;

    SceneBase* Current = nullptr;
    char* CurrentTitle = nullptr;

    SceneSelection& AddScene(const char* title, SceneBase* demo) {
		m_demoMap.insert({ title, demo });
        m_demoTitles.push_back(title);
        return *this;
	}

    // Sets the first demo as the current demo
    SceneSelection& Init() {
        
        if (!m_demoMap.empty()) {
			ChangeDemo(0);
		}

        return *this;
	}

    void ShowSelectionWindow() {
        ImGui::SetNextWindowPos(ImVec2(20, ImGui::GetIO().DisplaySize.y - 120));
        ImGui::SetNextWindowSize(ImVec2(440, 100));
        ImGui::Begin(SELECTION_HEADER, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        static int selectedItem = 0;

        if (ImGui::Combo("Select Demo", &selectedItem, m_demoTitles.data(), m_demoTitles.size())) {
            ChangeDemo(selectedItem);
        }

        ImGui::End();
    }

    void ShowSelectionChild()
    {
        ImGui::BeginChild("SelectionChild", ImVec2(400, 70), false, ImGuiWindowFlags_NoResize);

        static int selectedItem = 0;
        if (ImGui::Combo("Selection", &selectedItem, m_demoTitles.data(), m_demoTitles.size())) {
            ChangeDemo(selectedItem);
        }
        
        ImGui::EndChild();
    }

    const std::vector<const char*>& GetDemoTitles() const {
        return m_demoTitles;
    }

private:
    int m_currentDemoIndex = 0;
    std::vector<const char*> m_demoTitles = {};
    std::unordered_map<std::string, SceneBase*> m_demoMap = {};

    void ChangeDemo(const int index)
    {
		m_currentDemoIndex = index;
        const auto title = m_demoTitles[m_currentDemoIndex];
        CurrentTitle = const_cast<char*>(title);

        if (Current != nullptr) 
        {
            Current->OnExit();
        }

        Current = m_demoMap[title];
        Current->OnSetup();
        Current->Context.SetWindowTitle(title);

        Logger::LogDebug("Switched to Demo: %s", title);
	}
};

