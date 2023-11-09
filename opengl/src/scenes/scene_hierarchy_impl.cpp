#include "scene_hierarchy.h"

SceneHierarchy::SceneHierarchy()
{
    RootTransform.GuiDisplay = "Root";
}

void SceneHierarchy::show_transform_and_children(TransformComponent* transform, int& index)
{
    ImGui::PushID(index);
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding |
                               ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_SpanAvailWidth;

    // default open for root
    if (transform->GuiDisplay == "Root") {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }
    
    const boolean nodeOpen = ImGui::TreeNodeEx(
        transform->Uuid.c_str(), // id
        flags, transform->GuiDisplay == "Root" ?
        ICON_FK_GLOBE_W " Root" :
        ICON_FK_CIRCLE_THIN " %s", transform->GuiDisplay.c_str()); // display

    ImGui::PopID();

    if (nodeOpen)
    {
        transform->GuiShowControls();
        // recursively show children transforms
        for (TransformComponent* child : transform->Children)
        {
            show_transform_and_children(child, index);
        }
        ImGui::TreePop();
    }
    
    index++; // increment index
}

void SceneHierarchy::ShowDrawablesTree()
{
    int index = 0;
    show_transform_and_children(&RootTransform, index);
}

SceneHierarchy& SceneHierarchy::AddDrawable(Drawable* drawable)
{
    // no parent so set parent to global root
    if (!drawable->Transform.Parent)
    {
        drawable->Transform.SetParent(&RootTransform);
    }
    
    Drawables.push_back(drawable);
    return *this;
}

void SceneHierarchy::DrawAll(PerspectiveCamera& camera) const
{
    for (const auto & drawable : Drawables)
    {
        drawable->Draw(camera);
    }
}
