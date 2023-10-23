#pragma once

#include "../systems/drawable/drawable.h"
#include "../chess_pieces.h"

class Bishop : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces;
    
    Bishop(const Shader& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Bishop";
        Pieces = ChessPieces::GenerateBishop(color, Transform);
    }

    Bishop(const Shader& shader, const glm::vec3& color, const Texture& texture) : Drawable(shader, color, texture)
    {
        Transform.GuiDisplay = "Bishop";
        Pieces = ChessPieces::GenerateBishop(color, texture, Transform);
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

    virtual ~Bishop()
    {
        Pieces.clear();
    }
};