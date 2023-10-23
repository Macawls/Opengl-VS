#pragma once

#include "../systems/drawable/drawable.h"
#include "../chess_pieces.h"

class King : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces;
    
    King(const Shader& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "King";
        Pieces = ChessPieces::GenerateKing(color, Transform);
    }

    King(const Shader& shader, const glm::vec3& color, const Texture& texture) : Drawable(shader, color, texture)
    {
        Transform.GuiDisplay = "King";
        Pieces = ChessPieces::GenerateKing(color, texture, Transform);
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

    virtual ~King()
    {
        Pieces.clear();
    }
};