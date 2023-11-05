#pragma once

#include "../systems/drawable/drawable.h"
#include "..\chess_piece_generation.h"

class Rook : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces;

    Rook(const ShaderComponent& shader) : Drawable(shader)
    {
        Transform.GuiDisplay = "Rook";
        Pieces = ChessPieceGeneration::GenerateRook(Color, Transform);
    }
    
    Rook(const ShaderComponent& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Rook";
        Pieces = ChessPieceGeneration::GenerateRook(color, Transform);
    }

    Rook(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture) : Drawable(shader, color, texture)
    {
        Transform.GuiDisplay = "Rook";
        Pieces = ChessPieceGeneration::GenerateRook(color, Transform, texture);
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

    ~Rook() override
    {
        Pieces.clear();
    }
};