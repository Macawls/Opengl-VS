#include "chess_pieces.h"
#include "../../utils/pieces/impl/bishop.h"
#include "../../utils/pieces/impl/rook.h"
#include "../../utils/pieces/impl/king.h"
#include "../../utils/pieces/impl/knight.h"
#include "../../utils/pieces/impl/queen.h"


ChessPiecesScene::ChessPiecesScene(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings) 
	: SceneBase(context, camera, settings)
{
	constexpr auto pieceColor = glm::vec3(1.0f);
	const auto tex = Texture("src/resources/textures/ground.png");
	
	SceneHierarchy
	.AddDrawable(new Pawn(Shader(m_basicVertTex, m_basicFragTex), pieceColor, tex))
	.AddDrawable(new Rook(Shader(m_basicVertTex, m_basicFragTex), pieceColor, tex))
	.AddDrawable(new King(Shader(m_basicVertTex, m_basicFragTex), pieceColor, tex))
	.AddDrawable(new Queen(Shader(m_basicVertTex, m_basicFragTex), pieceColor, tex))
	.AddDrawable(new Bishop(Shader(m_basicVertTex, m_basicFragTex), pieceColor, tex))
	.AddDrawable(new Knight(Shader(m_basicVertTex, m_basicFragTex), pieceColor, tex));
}

void ChessPiecesScene::OnSetup()
{
	// place pieces in center
	const float gap = 2.0f;
	const int count = SceneHierarchy.Drawables.size();
	for (size_t i = 0; i < count; i++) {
		const float xPos = (i - (count - 1) / 2.0f) * gap;
		SceneHierarchy.Drawables[i]->Transform.SetPositionX(xPos);
	}

	m_camera.Transform.Position = glm::vec3(0.0f, 5.0f, 10.0f);
	m_camera.Transform.Rotation = glm::vec3(13.0f, 0.0f, 0.0f);
	m_camera.Settings.Fov = 80.0f;

	SceneHierarchy.RootTransform.Position = glm::vec3(-4.7, 1.9f, 0.0f);
}

void ChessPiecesScene::OnUpdate(const float deltaTime)
{
	glClearColor(ScreenClearColor.r, ScreenClearColor.g, ScreenClearColor.b, ScreenClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	const float rotateSpeed = 30.0f;
	SceneHierarchy.RootTransform.Rotation += glm::vec3(0.0f, rotateSpeed * deltaTime, 0.0f);
	SceneHierarchy.RootTransform.Rotation.y = fmod(SceneHierarchy.RootTransform.Rotation.y, 360.0f); // clamp

	SceneHierarchy.DrawAll(m_camera);
}

void ChessPiecesScene::OnGui()
{
	if (ImGui::BeginTabBar("Control Tabs"))
	{
		if (ImGui::BeginTabItem("Scene"))
		{
			SceneHierarchy.ShowDrawablesTree();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Camera"))
		{
			ImGui::SliderFloat3("Position", value_ptr(m_camera.Transform.Position), -3.0f, 15.0f);
			ImGui::SliderFloat3("Rotation##Cam", value_ptr(m_camera.Transform.Rotation), -180.0f, 180.0f);

			ImGui::SliderFloat("FOV", &m_camera.Settings.Fov, 5.0f, 140.0f);
			ImGui::SliderFloat("Speed", &m_camera.Settings.Speed, 2.0f, 10.0f);

			if (ImGui::Button("Reset"))
			{
				m_camera.Transform
					.Reset()
					.SetPosition(CAMERA_STARTING_POSITION);

				m_camera.Settings = CameraSettings();
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
