﻿#pragma once
#include "lights/directional.h"
#include "lights/point.h"
#include "lights/spot.h"

//#include "../drawable/billboard.h"

class SceneLightingData
{
public:
    DirectionalLight Directional;
    SpotLight Spot;
    
    std::vector<PointLight> PointLights;

    void GuiShowLightControls()
    {
        if (ImGui::CollapsingHeader("Directional"))
        {
            show_directional_light_controls(Directional);
        }

        if (ImGui::CollapsingHeader("Spot"))
        {
            show_spot_light_controls(Spot, 0);
        }
        
        for (int i = 0; i <  PointLights.size(); i++)
        {
            if (ImGui::CollapsingHeader(("Point Light " + std::to_string(i)).c_str()))
            {
                show_point_light_controls(PointLights[i], i);
            }
        }
    }


private:
    void show_spot_light_controls(SpotLight& light, const int id)
    {
        const auto i = std::to_string(id);

        // Position and Direction
        ImGui::Columns(2);
        ImGui::Text("Position");
        ImGui::NextColumn();
        ImGui::DragFloat3(("##SpotPosition" + i).c_str(), value_ptr(light.Position), 0.1f, -200, 200, "%.2f");
        ImGui::Columns(1);
        
        ImGui::Columns(2);
        ImGui::Text("Direction");
        ImGui::NextColumn();
        ImGui::DragFloat3(("##SpotDirection" + i).c_str(), value_ptr(light.Direction), 0.1f, -200, 200, "%.2f");
        ImGui::Columns(1);

        ImGui::Separator();

        // Cut Off and Outer Cut Off
        ImGui::Columns(2);
        ImGui::Text("Cutoff Angles");
        ImGui::NextColumn();
        ImGui::DragFloat(("##CutOff" + i).c_str(), &light.CutOff, 0.01f, 0.0f, 1.0f, "%.2f");
        ImGui::Columns(1);

        ImGui::Columns(2);
        ImGui::Text("Outer Cutoff");
        ImGui::NextColumn();
        ImGui::DragFloat(("##OuterCutOff" + i).c_str(), &light.OuterCutOff, 0.01f, 0.0f, 1.0f, "%.2f");
        ImGui::Columns(1);

        ImGui::Separator();

        // Attenuation
        ImGui::Columns(2);
        ImGui::Text("Attenuation");
        ImGui::NextColumn();
        ImGui::DragFloat(("##Constant" + i).c_str(), &light.Constant, 0.01f, 0.0f, 10.0f);
        ImGui::Columns(1);
        
        ImGui::Columns(2);
        ImGui::Text("Linear");
        ImGui::NextColumn();
        ImGui::DragFloat(("##Linear" + i).c_str(), &light.Linear, 0.001f, 0.0f, 1.0f);
        ImGui::Columns(1);

        ImGui::Columns(2);
        ImGui::Text("Quadratic");
        ImGui::NextColumn();
        ImGui::DragFloat(("##Quadratic" + i).c_str(), &light.Quadratic, 0.0001f, 0.0f, 0.1f);
        ImGui::Columns(1);

        ImGui::Separator();

        // Colors
        ImGui::Columns(2);
        ImGui::Text("Colors");
        ImGui::NextColumn();
        ImGui::ColorEdit3(("##Ambient" + i).c_str(), value_ptr(light.Ambient));
        ImGui::Columns(1);

        ImGui::Columns(2);
        ImGui::Text("Ambient");
        ImGui::NextColumn();
        ImGui::ColorEdit3(("##Diffuse" + i).c_str(), value_ptr(light.Diffuse));
        ImGui::Columns(1);

        ImGui::Columns(2);
        ImGui::Text("Diffuse");
        ImGui::NextColumn();
        ImGui::ColorEdit3(("##Specular" + i).c_str(), value_ptr(light.Specular));
        ImGui::Columns(1);

        ImGui::Separator();
    }


    

    void show_directional_light_controls(DirectionalLight& light)
    {

        ImGui::Columns(2);
        ImGui::SetColumnOffset(0, -130.0f);
        ImGui::Text("Direction");
        ImGui::NextColumn();
        ImGui::DragFloat3("##DirectionalDirection", value_ptr(light.Direction), 0.1f, -200, 200, "%.2f");
        ImGui::Columns(1);
        
        // Colors
        ImGui::Columns(2);
        ImGui::SetColumnOffset(0, -130.0f);
        ImGui::Text("Colors");
        ImGui::NextColumn();
        ImGui::ColorEdit3("##AmbientDirectional" , value_ptr(light.Ambient));
        ImGui::SameLine();
        ImGui::Text("Ambient");
        ImGui::ColorEdit3("##DiffuseDirectional", value_ptr(light.Diffuse));
        ImGui::SameLine();
        ImGui::Text("Diffuse");
        ImGui::ColorEdit3("##SpecularDirectional", value_ptr(light.Specular));
        ImGui::SameLine();
        ImGui::Text("Specular");
        ImGui::Columns(1);

        ImGui::Separator();
    }

    
    void show_point_light_controls(PointLight& light, const int id)
    {
        const auto i = std::to_string(id);
        
        light.Transform.GuiShowControlsPositionNoReset();
        ImGui::Separator();

        // Attenuation
        ImGui::Columns(2);
        ImGui::SetColumnOffset(0, -130.0f);
        ImGui::Text("Attenuation");
        ImGui::NextColumn();
        ImGui::SetNextItemWidth(80.0f);
        ImGui::DragFloat(("##Constant" + i).c_str(), &light.Constant, 0.01f, 0.0f, 10.0f);
        ImGui::SameLine();
        ImGui::Text("Constant");
        ImGui::SetNextItemWidth(80.0f);
        ImGui::DragFloat(("##Linear" + i).c_str(), &light.Linear, 0.001f, 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::Text("Linear");
        ImGui::SetNextItemWidth(80.0f);
        ImGui::DragFloat(("##Quadratic" + i).c_str(), &light.Quadratic, 0.0001f, 0.0f, 0.1f);
        ImGui::SameLine();
        ImGui::Text("Quadratic");
        ImGui::Columns(1);

        ImGui::Separator();
        
        // Colors
        ImGui::Columns(2);
        ImGui::SetColumnOffset(0, -130.0f);
        ImGui::Text("Colors");
        ImGui::NextColumn();
        ImGui::ColorEdit3(("##Ambient" + i).c_str(), value_ptr(light.Ambient));
        ImGui::SameLine();
        ImGui::Text("Ambient");
        ImGui::ColorEdit3(("##Diffuse" + i).c_str(), value_ptr(light.Diffuse));
        ImGui::SameLine();
        ImGui::Text("Diffuse");
        ImGui::ColorEdit3(("##Specular" + i).c_str(), value_ptr(light.Specular));
        ImGui::SameLine();
        ImGui::Text("Specular");
        ImGui::Columns(1);

        ImGui::Separator();
    }


};
