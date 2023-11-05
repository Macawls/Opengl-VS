#pragma once

#include "../systems/drawable/drawable.h"
#include "../chess_piece_generation.h"

class Bishop : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces;
    
    Bishop(const ShaderComponent& shader) : Drawable(shader)
    {
        Transform.GuiDisplay = "Bishop";
        Pieces = ChessPieceGeneration::GenerateBishop(Color, Transform);
    }

    Bishop(const ShaderComponent& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Bishop";
        Pieces = ChessPieceGeneration::GenerateBishop(color, Transform);
    }

    Bishop(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture) : Drawable(shader, color, texture)
    {
        Transform.GuiDisplay = "Bishop";
        Pieces = ChessPieceGeneration::GenerateBishop(color, Transform, texture);
    }
    
    void Draw(PerspectiveCamera& camera) override
    {
        for (size_t i = 0; i < Pieces.size(); i++)
        {
            Pieces[i]->Draw(camera);
        }
    }
    void Draw(PerspectiveCamera& camera, const glm::vec3& color) override
    {
        for (size_t i = 0; i < Pieces.size(); i++)
        {
            Pieces[i]->Draw(camera, color);
        }
    }

    ~Bishop() override
    {
        Pieces.clear();
    }
};