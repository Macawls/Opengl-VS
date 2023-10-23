#include "task_two.h"
#include <algorithm>
#include <random>

static glm::vec3 rainbow_color(const float multiplier)
{
    float red = 0.5f * (1.0f + std::sin(glm::two_pi<float>() * multiplier));
    float green = 0.5f * (1.0f + std::sin(glm::two_pi<float>() * multiplier + glm::two_pi<float>() / 3.0f));
    float blue = 0.5f * (1.0f + std::sin(glm::two_pi<float>() * multiplier + 2.0f * glm::two_pi<float>() / 3.0f));

    return { red, green, blue };
}

static float random01()
{
	return static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
}

static void RandomizeHeightOffsets(std::vector<Drawable*>& cells, float maxOffset)
{
    for (Drawable* cell : cells) {
        const float newY = random01() * maxOffset;

        cell->Transform.Children.front()->SetPositionY(newY);
        cell->Transform.SetPositionY(newY);
    }
}




TaskTwo::TaskTwo(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings) 
    : SceneBase(context, camera, settings)
{
    m_camera.Transform = m_cameraPositions[m_currentCamIndex];
    m_newCamPosition = m_camera.Transform;

    // Setup terrain
    m_terrain = new Terrain(m_terrainHeightPath,
        Shader(m_terrainVertSourceTex, m_terrainFragSourceTex),
        Texture(m_terrainTexturePath), terrain_params);

    m_terrain->Transform.Position.y = -2.0f;
    SceneHierarchy.AddDrawable(m_terrain);
    
    // Setup chess board border
    m_chessBorder = new Cube(Shader(m_basicVertTex, m_basicFragTex), glm::vec3(0.0f), Texture(borderTexturePath));
    m_chessBorder->Transform.SetParent(&SceneHierarchy.RootTransform);
    m_chessBorder->Transform.GuiDisplay = "Board";

    constexpr auto darkPieceColor = glm::vec3(0.64f, 0.40f, 0.22f);
    constexpr auto lightPieceColor = glm::vec3(0.89f, 0.79f, 0.54f);

    // Setup Board
    m_chessCells = generate_chess_board(pieceTexturePath, darkPieceColor, lightPieceColor);

    for (const auto cell : m_chessCells)
    {
        SceneHierarchy.AddDrawable(cell);
    }

    // width of board is 9
    m_chessBorder->Transform.Scale = glm::vec3(9.0f, 0.5f, 9.0f);
    // slightly lower avoid clipping
    m_chessBorder->Transform.Position = glm::vec3(0.0f, -0.2f, 0.0f);
}



void TaskTwo::OnSetup()
{
    m_camera.Reset();

    // farther plane to avoid clipping terrain
    m_camera.Settings.ClippingPlane = ClipPlane{ 0.1f, 300.0f };
    m_camera.Settings.Fov = 90.0f;

    m_camera.Transform = m_cameraPositions[m_currentCamIndex];
    m_newCamPosition = m_camera.Transform;

    ScreenClearColor = glm::vec4(0.5, 0.85, 1.0f, 1.0f);
    
    //RandomizeHeightOffsets(m_chessCells, m_cellMaxOffset);

    // more exciting
    //SceneHierarchy.RootTransform.SetRotationZ(10.0f);
}

void TaskTwo::OnUpdate(float deltaTime)
{
    static float time = 0.0f;
    time += deltaTime;
    
    glClearColor(ScreenClearColor.r, ScreenClearColor.g, ScreenClearColor.b, ScreenClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    // rotate scene
    constexpr float rotateSpeed = 20.0f;
    SceneHierarchy.RootTransform.Rotation.y += rotateSpeed * deltaTime;
    SceneHierarchy.RootTransform.Rotation.y = fmod(SceneHierarchy.RootTransform.Rotation.y, 360.0f); // clamp

    SceneHierarchy.DrawAll(m_camera);
    m_chessBorder->Draw(m_camera, rainbow_color(static_cast<float>(time * 0.2)));
}

void TaskTwo::OnGui()
{
	render_ui();
	handle_input();
}


std::vector<Drawable*> TaskTwo::generate_chess_board(const char* pieceTexturePath,
    const glm::vec3& darkPieceColor, const glm::vec3& lightPieceColor) const
{
    auto pieces = std::vector<Drawable*>();
    constexpr glm::vec3 boardCenterPos(3.5f, 0.0f, 3.5f);
    const auto pieceTexture = Texture(pieceTexturePath);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // White piece cord is divisible by 2
            glm::vec3 cellColor = (i + j) % 2 == 0 ?
                glm::vec3(1.0f) : glm::vec3(0.0f);

            // blocks
            const auto cell = new Cube(Shader(m_basicVert, m_basicFrag), cellColor);
            const glm::vec3 cubePosition(i - boardCenterPos.x, 0.0f, j - boardCenterPos.z);
            cell->Transform.GuiDisplay = "Cell (" + std::to_string(i) + ", " + std::to_string(j) + ")";
            cell->Transform.Position = cubePosition;
            // 0.5 in height
            cell->Transform.Scale = glm::vec3(1.0f, 0.5f, 1.0f);

            constexpr float posY = 0.5f;
            constexpr float scaleX = 0.5f;
            constexpr float scaleZ = 0.5f;

            // Add chess pieces
            // Pawns
            if (i == 1)
            {
                // Dark
                const auto pawn = new Pawn(Shader(m_basicVertTex, m_basicFragTex), darkPieceColor, pieceTexture);
                pawn->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                pieces.push_back(pawn);
            }
            else if (i == 6)
            {
                // Light
                const auto pawn = new Pawn(Shader(m_basicVertTex, m_basicFragTex), lightPieceColor, pieceTexture);
                pawn->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                pieces.push_back(pawn);
            }
            // Rooks, Knights, Bishops, King, Queen 
            else if (i == 0)
            {
                // Dark Pieces
                if (j == 0 || j == 7)
                {
                    // Dark Rooks
                    const auto rook = new Rook(Shader(m_basicVertTex, m_basicFragTex), darkPieceColor, pieceTexture);
                    rook->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(rook);
                }
                else if (j == 1 || j == 6)
                {
                    // Dark Knights
                    const auto knight = new Knight(Shader(m_basicVertTex, m_basicFragTex), darkPieceColor, pieceTexture);
                    knight->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(knight);
                }
                else if (j == 2 || j == 5)
                {
                    // Dark Bishops
                    const auto bishop = new Bishop(Shader(m_basicVertTex, m_basicFragTex), darkPieceColor, pieceTexture);
                    bishop->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(bishop);
                }
                else if (j == 3)
                {
                    // Dark Queen
                    const auto queen = new Queen(Shader(m_basicVertTex, m_basicFragTex), darkPieceColor, pieceTexture);
                    queen->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(queen);
                }
                else if (j == 4)
                {
                    // Dark King
                    const auto king = new King(Shader(m_basicVertTex, m_basicFragTex), darkPieceColor, pieceTexture);
                    king->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(king);
                }
            }
            else if (i == 7)
            {
                // Light Pieces
                if (j == 0 || j == 7)
                {
                    // Light Rooks
                    const auto rook = new Rook(Shader(m_basicVertTex, m_basicFragTex), lightPieceColor, pieceTexture);
                    rook->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(rook);
                }
                else if (j == 1 || j == 6)
                {
                    // Light Knights
                    const auto knight = new Knight(Shader(m_basicVertTex, m_basicFragTex), lightPieceColor, pieceTexture);
                    knight->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(knight);
                }
                else if (j == 2 || j == 5)
                {
                    // Light Bishops
                    const auto bishop = new Bishop(Shader(m_basicVertTex, m_basicFragTex), lightPieceColor, pieceTexture);
                    bishop->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(bishop);
                }
                else if (j == 3)
                {
                    // Light Queen
                    const auto queen = new Queen(Shader(m_basicVertTex, m_basicFragTex), lightPieceColor, pieceTexture);
                    queen->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(queen);
                }
                else if (j == 4)
                {
                    // Light King
                    const auto king = new King(Shader(m_basicVertTex, m_basicFragTex), lightPieceColor, pieceTexture);
                    king->Transform.SetScaleX(scaleX).SetScaleZ(scaleZ).SetPositionY(posY).SetParent(&cell->Transform);
                    pieces.push_back(king);
                }
            }
            
            pieces.push_back(cell);
        }
    }

    return pieces;
}

void TaskTwo::next_cam_pos()
{
    m_currentCamIndex = (m_currentCamIndex + 1) % 3;
    m_newCamPosition = m_cameraPositions[m_currentCamIndex];
    check_new_cam_pos();
}

void TaskTwo::prev_cam_pos()
{
    m_currentCamIndex = (m_currentCamIndex - 1 + 3) % 3;
    m_newCamPosition = m_cameraPositions[m_currentCamIndex];
    check_new_cam_pos();
}

void TaskTwo::check_new_cam_pos() {
    TransformComponent current = m_camera.Transform;
    const float duration = 0.5f;

    // avoid setting position every frame
    /*
    if (m_newCamPosition != current)
    {
        if (CAM_TWEEN_PLAYING) {
			m_cameraTweenPosX->Pause();
			m_cameraTweenPosY->Pause();
			m_cameraTweenPosZ->Pause();
		}

        m_cameraTweenPosX = pTween::pTween(&m_camera.Transform.Position.x)
            ->To(m_newCamPosition.Position.x)
            ->Duration(duration)
            ->Play()
            ->OnStartCallBack([]()
                {
                    CAM_TWEEN_PLAYING = true;
                })
            ->OnEndCallBack([]()
                {
                    CAM_TWEEN_PLAYING = false;
                });

        m_cameraTweenPosY = pTween::pTween(&m_camera.Transform.Position.y)
            ->To(m_newCamPosition.Position.y)
            ->Duration(duration)
            ->Play();

        m_cameraTweenPosZ = pTween::pTween(&m_camera.Transform.Position.z)
            ->To(m_newCamPosition.Position.z)
            ->Duration(duration)
            ->Play();

        m_cameraTweenRotX = pTween::pTween(&m_camera.Transform.Rotation.x)
			->To(m_newCamPosition.Rotation.x)
			->Duration(duration)
			->Play();

        m_cameraTweenRotY = pTween::pTween(&m_camera.Transform.Rotation.y)
            ->To(m_newCamPosition.Rotation.y)
            ->Duration(duration)
            ->Play();

        m_cameraTweenRotZ = pTween::pTween(&m_camera.Transform.Rotation.z)
            ->To(m_newCamPosition.Rotation.z)
            ->Duration(duration)
            ->Play();
    }
    */
}

// flags are used to avoid setting every frame
void TaskTwo::handle_input() {
    static bool right = false;
    static bool left = false;
    const int rightState = glfwGetKey(m_window, GLFW_KEY_RIGHT);
    const int leftState = glfwGetKey(m_window, GLFW_KEY_LEFT);

    if (rightState == GLFW_PRESS && !right) {
        next_cam_pos();
        right = true;
    }
    else if (rightState != GLFW_PRESS) {
        right = false;
    }

    if (leftState == GLFW_PRESS && !left) {
        prev_cam_pos();
        left = true;
    }
    else if (leftState != GLFW_PRESS) {
        left = false;
    }
}

void TaskTwo::play_cell_anim()
{
    
    for (Drawable* cell : m_chessCells)
    {
        pTween::pTween(&cell->Transform.Position.y)
            ->From(30.0f)
            ->To(0.0f)
            ->Duration(static_cast<float>(static_cast<double>(random01() * 1.5f) + 2.0))
            ->Delay(random01() * 1.0f)
            ->Transition(pTween::pTweenTransitions::EaseInOutBounce)
            ->Play();
    };
    
}

void TaskTwo::render_ui()
{
    if (ImGui::BeginTabBar("Control Tabs"))
    {
        if (ImGui::BeginTabItem("Scene"))
        {
            SceneHierarchy.ShowDrawablesTree();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Terrain"))
        {
            static TerrainOptions userOptions = terrain_params;

            ImGui::Text("Options");
                   
            ImGui::SliderInt("X", &userOptions.DesiredWidth, 20, 80);
            ImGui::SliderInt("Z", &userOptions.DesiredLength, 10, 80);
            ImGui::SliderFloat("Max Height", &userOptions.MaxHeight, 10.0f, 50.0f);

            if (ImGui::Button("Apply##TerrainOptions"))
            {
                m_terrain = new Terrain(
                    m_terrainHeightPath,
                    Shader(m_terrainVertSource, m_terrainFragSource), userOptions);
            }

            ImGui::Spacing();
            if (ImGui::Button("Reset##TerrainOptions"))
            {
                userOptions = terrain_params;

                m_terrain = new Terrain(
                    m_terrainHeightPath, 
                    Shader(m_terrainVertSource, m_terrainFragSource), userOptions); 
            }

            m_terrain->Transform.ShowControls();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Chessboard"))
        {
            ImGui::Text("Use offset to change max height");
            ImGui::SliderFloat("Max Offset", &m_cellMaxOffset, 0.0f, 1.0f);

            if (ImGui::Button("Play Animation")) play_cell_anim();
            //if (ImGui::Button("Randomize Cell Heights")) RandomizeHeightOffsets(m_chessCells, m_cellMaxOffset);
            //if (ImGui::Button("Reset")) RandomizeHeightOffsets(m_cells, 0.0f);
                    

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera"))
        {
            ImGui::Checkbox("Use Unlocked Mode", &m_cameraUnlocked);


            if (m_cameraUnlocked)
            {
                m_camera.Transform.ShowControlsExcludeScale();

                ImGui::SliderFloat("FOV##Cam", &m_camera.Settings.Fov, 5.0f, 140.0f);
                ImGui::SliderFloat("Speed##Cam", &m_camera.Settings.Speed, 2.0f, 10.0f);

                if (ImGui::Button("Reset##CamTransform"))
                {
                    m_camera.Transform
                        .Reset()
                        .SetPosition(CAMERA_STARTING_POSITION);

                    m_camera.Settings = CameraSettings();
                }
            }
            else
            {
                ImGui::Text("Camera Position [%d]", m_currentCamIndex);
                if (ImGui::Button("Next")) next_cam_pos();
                ImGui::SameLine(); 
                if (ImGui::Button("Previous")) prev_cam_pos();

                if (ImGui::Button("Reset"))
                {
                    m_currentCamIndex = 0;
                }

            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Background"))
        {
            ImGui::ColorEdit4("Background", glm::value_ptr(ScreenClearColor));
            ImGui::EndTabItem();

            if (ImGui::Button("Reset"))
            {
                ScreenClearColor = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);
            }
        }

        ImGui::EndTabBar();
    }
}
