﻿#pragma once

#include "../systems/drawable/drawable.h"
#include "../chess_piece_generation.h"

class Pawn : public Drawable
{
public:
    using Drawable::Drawable;
    std::vector<Drawable*> Pieces{};

    Pawn(const ShaderComponent& shader) : Drawable(shader)
    {
        Transform.GuiDisplay = "Pawn";
        Pieces = ChessPieceGeneration::GeneratePawn(Color, Transform);
    }
    
    Pawn(const ShaderComponent& shader, const glm::vec3& color) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Pawn";
        Pieces = ChessPieceGeneration::GeneratePawn(color, Transform);
    }

    Pawn(const ShaderComponent& shader, const glm::vec3& color, const TextureComponent& texture) : Drawable(shader, color)
    {
        Transform.GuiDisplay = "Pawn";
        Pieces = ChessPieceGeneration::GeneratePawn(color, Transform, texture);
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

    ~Pawn() override
    {
        Pieces.clear();
    }
};