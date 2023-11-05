#pragma once

#include "../systems/drawable/drawable.h"
#include "..\chess_piece_generation.h"

class Queen : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces;

    Queen(const ShaderComponent& shader) : Drawable(shader)
    {
        Transform.GuiDisplay = "Queen";
        Pieces = ChessPieceGeneration::GenerateQueen(Color, Transform);
    }
    
    Queen(const ShaderComponent& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Queen";
        Pieces = ChessPieceGeneration::GenerateQueen(color, Transform);
    }

    Queen(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Queen";
        Pieces = ChessPieceGeneration::GenerateQueen(color, Transform, texture);
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

    ~Queen() override
    {
        Pieces.clear();
    }
};