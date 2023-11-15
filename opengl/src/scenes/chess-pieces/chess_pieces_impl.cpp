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
	const auto pieceTexture = TextureComponent(texPath);
	
	const auto pawn = new Pawn(ShaderComponent(m_basicVertTex, m_basicFragTex), pieceColor, pieceTexture);
	const auto rook = new Rook(ShaderComponent(m_basicVertTex, m_basicFragTex), pieceColor, pieceTexture);
	const auto king = new King(ShaderComponent(m_basicVertTex, m_basicFragTex), pieceColor, pieceTexture);
	const auto queen = new Queen(ShaderComponent(m_basicVertTex, m_basicFragTex), pieceColor, pieceTexture);
	const auto bishop = new Bishop(ShaderComponent(m_basicVertTex, m_basicFragTex), pieceColor, pieceTexture);
	const auto knight = new Knight(ShaderComponent(m_basicVertTex, m_basicFragTex), pieceColor, pieceTexture);
	
	SceneHierarchy.AddDrawable(pawn)
	.AddDrawable(rook)
	.AddDrawable(king)
	.AddDrawable(queen)
	.AddDrawable(bishop)
	.AddDrawable(knight);
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
	
	LoadCamSnapShot();
}

void ChessPiecesScene::OnExit()
{
	SaveCamSnapshot();
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
			SceneHierarchy.GuiShowDrawablesTree();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Camera"))
		{
			m_camera.GuiShowControls();
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

