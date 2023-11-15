#pragma once
#include "../systems/rendering/perspective_camera.h"
#include "../systems/settings/render_settings.h"
#include "../systems/window/window_context.h"
#include "..\systems\shader\shader_component.h"
#include "../systems/settings/render_settings.h"
#include "../systems/rendering/transform.h"
#include "../systems/rendering/perspective_camera.h"
#include "../resources/fonts/forkawesome-icons.h"

// drawables
#include "../systems/drawable/impl/cone.h"
#include "../systems/drawable/impl/cone.h"
#include "../systems/drawable/impl/cone.h"

/**
 * \brief Keeps a list of drawables internally and manages displaying and showing controls
 */
class SceneHierarchy
{
public:
    SceneHierarchy();
    TransformComponent RootTransform; // global root
    std::vector<Drawable*> Drawables{};
    SceneHierarchy& AddDrawable(Drawable* drawable);
    void DrawAll(PerspectiveCamera& perspectiveCamera) const;

    // gui
    void GuiShowDrawablesTree();
private:
    void DisplayTransformTree(TransformComponent* transform, int& index);
};
