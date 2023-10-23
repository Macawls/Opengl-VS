#include "task_one.h"


static glm::vec3 rainbow_color(float multiplier)
{
    float red = 0.5f * (1.0f + std::sin(glm::two_pi<float>() * multiplier));
    float green = 0.5f * (1.0f + std::sin(glm::two_pi<float>() * multiplier + glm::two_pi<float>() / 3.0f));
    float blue = 0.5f * (1.0f + std::sin(glm::two_pi<float>() * multiplier + 2.0f * glm::two_pi<float>() / 3.0f));

    return glm::vec3(red, green, blue);
}

static float random01()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

static void RandomizeHeightOffsets(std::vector<Cube>& cells, float maxOffset)
{
    for (Cube& cell : cells) {
        glm::vec3 newPosition = cell.Transform.Position;
        newPosition.y = (random01() * maxOffset);
        
        cell.Transform.SetPosition(newPosition);
    }
}

static bool cellAnimationBusy = false;

const char* TERRAIN_PATH = "src/resources/images/terrain/berg-river.png";
const float TERRAIN_TWEEN_DURATION = 40.0f;
const TerrainOptions TERRAIN_PARAMS = { 200, 200, 30.0f };

const double CUBE_WAIT_TIME = 1.0;
double timeToRenderCubes = CUBE_WAIT_TIME;

TaskOne::TaskOne(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings) 
    : SceneBase(context, camera, settings)
{
    m_camera.Transform = m_cameraPositions[m_currentCamIndex];
    m_newCamPosition = m_camera.Transform;

    // Setup terrain
    auto terrainShader = Shader(m_terrainVertSource, m_terrainFragSource);
    m_terrain = new Terrain(TERRAIN_PATH, terrainShader, TERRAIN_PARAMS);
    m_terrain->Transform.Position.y = -2.0f;

    // Setup chess board
    m_cells = generate_chess_board();
    
    // Setup border
    auto borderShader = Shader(m_cubeVertSource, m_cubeFragSource);
    m_chessBorder = new Cube(borderShader, glm::vec3(0.0f, 0.0f, 0.0f));

    // width of board is 9
    m_chessBorder->Transform.Scale = glm::vec3(9.0f, 0.5f, 9.0f);
    // slightly lower avoid clipping
    m_chessBorder->Transform.Position = glm::vec3(0.0f, -0.2f, 0.0f);
}


void TaskOne::OnSetup()
{
    m_camera.Reset();

    // farther plane to avoid clipping terrain
    m_camera.Settings.ClippingPlane = ClipPlane{ 0.1f, 500.0f };
    m_camera.Settings.Fov = 90.0f;

    m_camera.Transform = m_cameraPositions[m_currentCamIndex];
    m_newCamPosition = m_camera.Transform;
    
    RandomizeHeightOffsets(m_cells, m_cellMaxOffset);
    
    // Rotate Terrain
    m_terrainTween = pTween::pTween(&m_terrain->Transform.Rotation.y)
        ->To(360.0f)
        ->Duration(TERRAIN_TWEEN_DURATION)
        ->Play();


    play_cell_anim();
}

void TaskOne::OnUpdate(float deltaTime)
{
    glClearColor(ScreenClearColor.r, ScreenClearColor.g, ScreenClearColor.b, ScreenClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (glfwGetTime() > timeToRenderCubes)
    {
        for (Cube& cell : m_cells)
        {
            cell.Draw(m_camera);
        }
    }

    m_chessBorder->Draw(m_camera, rainbow_color(static_cast<float>(glfwGetTime() * 0.2)));
    m_terrain->Draw(m_camera);

    // loop tween
    if (m_terrain->Transform.Rotation.y > 359.0f) {

        m_terrain->Transform.Rotation.y = 360.0f;
        m_terrainTween->Pause();
        m_terrain->Transform.Rotation.y = 0.0f;

        m_terrainTween = pTween::pTween(&m_terrain->Transform.Rotation.y)
            ->To(360.0f)
            ->Duration(TERRAIN_TWEEN_DURATION)
            ->Play();
    }
}

void TaskOne::OnGui()
{
	render_ui();
	handle_input();
}

std::vector<Cube> TaskOne::generate_chess_board()
{
    std::vector<Cube> pieces = std::vector<Cube>();
    glm::vec3 boardCenterPos(3.5f, 0.0f, 3.5f);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            glm::vec3 color;
            
            // White piece cord is divisible by 2
            color = (i + j) % 2 == 0 ? glm::vec3(1.0f, 1.0f, 1.0f) : color = glm::vec3(0.0f, 0.0f, 0.0f);


            Shader shader = Shader()
                .SetVertexSource(m_cubeVertSource)
                .SetFragmentSource(m_cubeFragSource)
                .Compile();
            
            Cube cube = Cube(shader, color);

            glm::vec3 cubePosition(i - boardCenterPos.x, 0.0f, j - boardCenterPos.z);
            cube.Transform.Position = cubePosition;
            
            // 0.5 in height
            cube.Transform.Scale = glm::vec3(1.0f, 0.5f, 1.0f);
            
            pieces.push_back(cube);
        }
    }

    return pieces;
}

void TaskOne::next_cam_pos()
{
    m_currentCamIndex = (m_currentCamIndex + 1) % 3;
    m_newCamPosition = m_cameraPositions[m_currentCamIndex];
    check_new_cam_pos();
}

void TaskOne::prev_cam_pos()
{
    m_currentCamIndex = (m_currentCamIndex - 1 + 3) % 3;
    m_newCamPosition = m_cameraPositions[m_currentCamIndex];
    check_new_cam_pos();
}

void TaskOne::check_new_cam_pos() {
    TransformComponent current = m_camera.Transform;
    const float duration = 0.5f;

    // avoid setting position every frame
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
}

// flags are used to avoid setting every frame
void TaskOne::handle_input() {
    static bool right = false;
    static bool left = false;
    int rightState = glfwGetKey(m_window, GLFW_KEY_RIGHT);
    int leftState = glfwGetKey(m_window, GLFW_KEY_LEFT);

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

void TaskOne::play_cell_anim()
{
    for (Cube& cell : m_cells)
    {
        float randomHeight = random01() * m_cellMaxOffset;
            
        pTween::pTween(&cell.Transform.Position.y)
            ->From(30.0f)
            ->To(randomHeight)
            ->Duration(static_cast<float>(static_cast<double>(random01() * 1.5f) + 2.0))
            ->Delay(random01() * 1.0f)
            ->Transition(pTween::pTweenTransitions::EaseInOutBounce)
            ->Play();
    };
}

void TaskOne::render_ui()
{
    if (ImGui::BeginTabBar("Control Tabs"))
    {
        if (ImGui::BeginTabItem("Terrain"))
        {
            static TerrainOptions userOptions = TERRAIN_PARAMS;

            ImGui::Text("Options");
                   
            ImGui::SliderInt("X", &userOptions.DesiredWidth, 20, 80);
            ImGui::SliderInt("Z", &userOptions.DesiredLength, 10, 80);
            ImGui::SliderFloat("Max Height", &userOptions.MaxHeight, 10.0f, 50.0f);

            if (ImGui::Button("Apply##TerrainOptions"))
            {
                m_terrain = new Terrain(
                    TERRAIN_PATH,
                    Shader(m_terrainVertSource, m_terrainFragSource), userOptions);
            }

            ImGui::Spacing();
            if (ImGui::Button("Reset##TerrainOptions"))
            {
                userOptions = TERRAIN_PARAMS;

                m_terrain = new Terrain(
                    TERRAIN_PATH, 
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
            if (ImGui::Button("Randomize Cell Heights")) RandomizeHeightOffsets(m_cells, m_cellMaxOffset);
            if (ImGui::Button("Reset")) RandomizeHeightOffsets(m_cells, 0.0f);
                    

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
