#pragma once

#include "../systems/drawable/drawable.h"
#include "../chess_piece_generation.h"

class King : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces;
    
    King(const ShaderComponent& shader) : Drawable(shader)
    {
        Transform.GuiDisplay = "King";
        Pieces = ChessPieceGeneration::GenerateKing(Color, Transform);
    }
    
    King(const ShaderComponent& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "King";
        Pieces = ChessPieceGeneration::GenerateKing(color, Transform);
    }

    King(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture) : Drawable(shader, color, texture)
    {
        Transform.GuiDisplay = "King";
        Pieces = ChessPieceGeneration::GenerateKing(color, Transform, texture);
    }
    
    void Draw(PerspectiveCamera& camera) override
    {
        for (const auto piece : Pieces)
        {
            piece->Draw(camera);
        }
    }
    void Draw(PerspectiveCamera& camera, const glm::vec3& color) override
    {
        for (const auto piece : Pieces)
        {
            piece->Draw(camera, color);
        }
    }

    ~King() override
    {
        Pieces.clear();
    }
};