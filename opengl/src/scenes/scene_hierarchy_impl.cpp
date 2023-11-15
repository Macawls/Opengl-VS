#include "scene_hierarchy.h"

SceneHierarchy::SceneHierarchy()
{
    RootTransform.GuiDisplay = "Root";
}



void SceneHierarchy::GuiShowDrawablesTree()
{
    int index = 0;
    DisplayTransformTree(&RootTransform, index);
}

/**
 * \brief Shows the ImGUI Transform Tree
 * \param transform Transform to start the tree from
 * \param index Applies unique IDs to each node, ref is mutated
 */
void SceneHierarchy::DisplayTransformTree(TransformComponent* transform, int& index)
{
    ImGui::PushID(index);

    // styling and behaviour
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding |
                               ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_SpanAvailWidth;

    // default open for root node
    if (transform->GuiDisplay == "Root")
        flags |= ImGuiTreeNodeFlags_DefaultOpen; // add to existing flags
    
    // Display text and node ID, ICON_FK for icons
    const char* displayText = transform->GuiDisplay == "Root" ?
        ICON_FK_GLOBE_W " Root" : ICON_FK_CIRCLE_THIN " %s";

    const char* nodeId = transform->Uuid.c_str(); // UUID of Transform

    // Create the tree node
    const bool nodeOpen = ImGui::TreeNodeEx(
        nodeId,                             // id
        flags,                              // flags
        displayText,                        // display text
        transform->GuiDisplay.c_str());     // additional formatting if not root

    // pop after pushing
    ImGui::PopID();

    // If currently selected, display controls and children's controls recursively
    if (nodeOpen)
    {
        transform->GuiShowControls();
        for (TransformComponent* child : transform->Children)
            DisplayTransformTree(child, index);

        ImGui::TreePop();   
    }
    
    index++; // increment index so node is unique
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
