#pragma once
#include "demo-base.h"
#include "../resources/fonts/forkawesome-icons.h"

static const char* INFO_HEADER = ICON_FK_WINDOW_MAXIMIZE " Demo Selection";

// Handles demo selection and switching
class DemoSelection {
public:
    DemoSelection() = default;

    DemoBase* Current = nullptr;
    char* CurrentTitle = nullptr;

    DemoSelection& AddDemo(const char* title, DemoBase* demo) {
		m_demoMap.insert({ title, demo });
        m_demoTitles.push_back(title);
        return *this;
	}

    // Sets the first demo as the current demo
    DemoSelection& Init() {
        
        if (!m_demoMap.empty()) {
			ChangeDemo(0);
		}

        return *this;
	}

    void ShowSelectionWindow() {
        ImGui::SetNextWindowPos(ImVec2(20, ImGui::GetIO().DisplaySize.y - 120));
        ImGui::SetNextWindowSize(ImVec2(440, 100));
        ImGui::Begin(INFO_HEADER, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        static int selectedItem = 0;

        if (ImGui::Combo("Select Demo", &selectedItem, m_demoTitles.data(), m_demoTitles.size())) {
            ChangeDemo(selectedItem);
        }

        ImGui::End();
    }

    const std::vector<const char*>& GetDemoTitles() const {
        return m_demoTitles;
    }

private:
    int m_currentDemoIndex = 0;
    std::vector<const char*> m_demoTitles = {};
    std::unordered_map<std::string, DemoBase*> m_demoMap = {};

    void ChangeDemo(const int index)
    {
		m_currentDemoIndex = index;
        const auto title = m_demoTitles[m_currentDemoIndex];
        CurrentTitle = const_cast<char*>(title);
		
        Current = m_demoMap[title];
        Current->OnSetup();
        Current->Context.SetWindowTitle(title);

        Logger::LogDebug("Switched to Demo: %s", title);
	}
};

