#pragma once

#include "../systems/drawable/drawable.h"
#include "../chess_pieces.h"

class Pawn : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces;
    
    Pawn(const Shader& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Pawn";
        Pieces = ChessPieces::GeneratePawn(color, Transform);
    }

    Pawn(const Shader& shader, const glm::vec3& color, const Texture& texture) : Drawable(shader, color, texture)
    {
        Transform.GuiDisplay = "Pawn";
        Pieces = ChessPieces::GeneratePawn(color, texture, Transform);
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

    virtual ~Pawn()
    {
        Pieces.clear();
    }
};