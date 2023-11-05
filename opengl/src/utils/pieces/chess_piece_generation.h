#pragma once

#include "../systems/drawable/impl/cone.h"
#include "../systems/drawable/impl/cylinder.h"
#include "../systems/drawable/impl/cube.h"
#include "../systems/drawable/impl/sphere.h"
#include "../systems/drawable/drawable.h"

class ChessPieceGeneration {
public:
    static std::vector<Drawable*> GeneratePawn(const glm::vec3& color, TransformComponent& parent)
    {
        std::vector<Drawable*> pieces;

        const auto sphere = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cone = new Cone(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        sphere->Transform.SetPositionY(1.8f).SetParent(&parent);
        cylinderTop->Transform.SetPositionY(1.3f).SetRotationX(90.f).SetScaleZ(0.2f).SetParent(&parent);
        cone->Transform.SetPositionY(0.5f).SetRotationX(90.f).SetScale(glm::vec3(1.5f)).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5f)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(sphere);
        pieces.push_back(cylinderTop);
        pieces.push_back(cone);
        pieces.push_back(cylinderBase);

        return pieces;
    }
    
    static std::vector<Drawable*> GeneratePawn(const glm::vec3& color, TransformComponent& parent, const TextureComponent& texture)
    {
        std::vector<Drawable*> pieces;

        const auto sphere = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cone = new Cone(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        sphere->Transform.SetPositionY(1.8f).SetParent(&parent);
        cylinderTop->Transform.SetPositionY(1.3f).SetRotationX(90.f).SetScaleZ(0.2f).SetParent(&parent);
        cone->Transform.SetPositionY(0.5f).SetRotationX(90.f).SetScale(glm::vec3(1.5f)).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5f)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(sphere);
        pieces.push_back(cylinderTop);
        pieces.push_back(cone);
        pieces.push_back(cylinderBase);

        return pieces;
    }

    static std::vector<Drawable*> GenerateRook(const glm::vec3& color, TransformComponent& parent)
    {
        std::vector<Drawable*> pieces;
        
        const auto cubeL = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cubeR = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cubeF = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cubeB = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        cubeL->Transform.SetPositionY(2.5f).SetPositionX(-0.4f).SetScale(glm::vec3(0.3f)).SetScaleX(0.2f).SetParent(&parent);
        cubeR->Transform.SetPositionY(2.5f).SetPositionX(0.4f).SetScale(glm::vec3(0.3f)).SetScaleX(0.2f).SetParent(&parent);
        cubeF->Transform.SetPositionY(2.5f).SetPositionZ(0.4f).SetScale(glm::vec3(0.3f)).SetScaleZ(0.2f).SetParent(&parent);
        cubeB->Transform.SetPositionY(2.5f).SetPositionZ(-0.4f).SetScale(glm::vec3(0.3f)).SetScaleZ(0.2f).SetParent(&parent);

        cylinderTop->Transform.SetPositionY(2.3f).SetRotationX(90.f).SetScaleZ(0.2f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.2f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(2.2f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(cubeL);
        pieces.push_back(cubeR);
        pieces.push_back(cubeF);
        pieces.push_back(cubeB);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);
        return pieces;
    }

    static std::vector<Drawable*> GenerateRook(const glm::vec3& color, TransformComponent& parent, const TextureComponent& texture)
    {
        std::vector<Drawable*> pieces;
        
        const auto cubeL = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cubeR = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cubeF = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cubeB = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        cubeL->Transform.SetPositionY(2.5f).SetPositionX(-0.4f).SetScale(glm::vec3(0.3f)).SetScaleX(0.2f).SetParent(&parent);
        cubeR->Transform.SetPositionY(2.5f).SetPositionX(0.4f).SetScale(glm::vec3(0.3f)).SetScaleX(0.2f).SetParent(&parent);
        cubeF->Transform.SetPositionY(2.5f).SetPositionZ(0.4f).SetScale(glm::vec3(0.3f)).SetScaleZ(0.2f).SetParent(&parent);
        cubeB->Transform.SetPositionY(2.5f).SetPositionZ(-0.4f).SetScale(glm::vec3(0.3f)).SetScaleZ(0.2f).SetParent(&parent);

        cylinderTop->Transform.SetPositionY(2.3f).SetRotationX(90.f).SetScaleZ(0.2f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.2f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(2.2f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(cubeL);
        pieces.push_back(cubeR);
        pieces.push_back(cubeF);
        pieces.push_back(cubeB);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);
        return pieces;
    }

    static std::vector<Drawable*> GenerateKing(const glm::vec3& color, TransformComponent& parent)
    {
        std::vector<Drawable*> pieces;
        
        const auto cubeVert = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cubeHor = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        cubeVert->Transform.SetRotationY(180.0f).SetPositionY(3.1f).SetScale(glm::vec3(0.3f)).SetScaleY(0.8f).SetParent(&parent);
        cubeHor->Transform.SetRotationY(180.0f).SetPositionY(3.1f).SetScale(glm::vec3(0.3f)).SetScaleZ(0.8f).SetParent(&parent);

        cylinderTop->Transform.SetPositionY(2.5f).SetRotationX(90.f).SetScaleZ(0.4f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.4f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(2.4f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(cubeVert);
        pieces.push_back(cubeHor);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);
        return pieces;
    }

    static std::vector<Drawable*> GenerateKing(const glm::vec3& color, TransformComponent& parent, const TextureComponent& texture)
    {
        std::vector<Drawable*> pieces;
        
        const auto cubeVert = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cubeHor = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        cubeVert->Transform.SetRotationY(180.0f).SetPositionY(3.1f).SetScale(glm::vec3(0.3f)).SetScaleY(0.8f).SetParent(&parent);
        cubeHor->Transform.SetRotationY(180.0f).SetPositionY(3.1f).SetScale(glm::vec3(0.3f)).SetScaleZ(0.8f).SetParent(&parent);

        cylinderTop->Transform.SetPositionY(2.5f).SetRotationX(90.f).SetScaleZ(0.4f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.4f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(2.4f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(cubeVert);
        pieces.push_back(cubeHor);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);
        return pieces;
    }

    static std::vector<Drawable*> GenerateQueen(const glm::vec3& color, TransformComponent& parent)
    {
        std::vector<Drawable*> pieces;

        const auto sphereTop = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto sphere = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cone = new Cone(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);

        
        cone->Transform.SetPositionY(2.5f).SetRotationX(-90.0f).SetScale(glm::vec3(0.9f)).SetParent(&parent);
        sphere->Transform.SetPositionY(3.0f).SetScale(glm::vec3(0.7f)).SetScaleY(0.4f).SetParent(&parent);
        sphereTop->Transform.SetPositionY(3.3f).SetScale(glm::vec3(0.4f)).SetParent(&parent);
        
        cylinderTop->Transform.SetPositionY(2.5f).SetRotationX(90.f).SetScaleZ(0.4f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.4f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(2.4f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(sphere);
        pieces.push_back(sphereTop);
        pieces.push_back(cone);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);
        return pieces;
    }

    static std::vector<Drawable*> GenerateQueen(const glm::vec3& color, TransformComponent& parent, const TextureComponent& texture)
    {
        std::vector<Drawable*> pieces;

        const auto sphereTop = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto sphere = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cone = new Cone(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);

        
        cone->Transform.SetPositionY(2.5f).SetRotationX(-90.0f).SetScale(glm::vec3(0.9f)).SetParent(&parent);
        sphere->Transform.SetPositionY(3.0f).SetScale(glm::vec3(0.7f)).SetScaleY(0.4f).SetParent(&parent);
        sphereTop->Transform.SetPositionY(3.3f).SetScale(glm::vec3(0.4f)).SetParent(&parent);
        
        cylinderTop->Transform.SetPositionY(2.5f).SetRotationX(90.f).SetScaleZ(0.4f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.4f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(2.4f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);
        
        pieces.push_back(sphere);
        pieces.push_back(sphereTop);
        pieces.push_back(cone);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);
        return pieces;
    }

    static std::vector<Drawable*> GenerateBishop(const glm::vec3& color, TransformComponent& parent)
    {
        std::vector<Drawable*> pieces;

        const auto coneT = new Cone(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto sphere1 = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto sphere2 = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        const auto coneB = new Cone(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);

        coneT->Transform.SetPositionY(1.7f).SetRotationX(270.0f).SetParent(&parent);
        sphere1->Transform.SetPositionY(2.9f).SetScale(glm::vec3(0.2f)).SetParent(&parent);
        sphere2->Transform.SetPositionY(2.4f).SetScale(glm::vec3(1.2f)).SetScaleY(0.3f).SetParent(&parent);
        coneB->Transform.SetPositionY(2.7f).SetScale(glm::vec3(0.8f)).SetScaleZ(0.35f).SetRotationX(90.0f).SetParent(&parent);


        cylinderTop->Transform.SetPositionY(2.3f).SetRotationX(90.f).SetScaleZ(0.2f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.1f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(1.9f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);

        pieces.push_back(coneT);
        pieces.push_back(sphere1);
        pieces.push_back(sphere2);
        pieces.push_back(coneB);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);


        return pieces;
    }

    static std::vector<Drawable*> GenerateBishop(const glm::vec3& color, TransformComponent& parent, const TextureComponent& texture)
    {
        std::vector<Drawable*> pieces;

        const auto coneT = new Cone(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto sphere1 = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto sphere2 = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        const auto coneB = new Cone(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        const auto cylinderTop = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderMid = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);

        coneT->Transform.SetPositionY(1.7f).SetRotationX(270.0f).SetParent(&parent);
        sphere1->Transform.SetPositionY(2.9f).SetScale(glm::vec3(0.2f)).SetParent(&parent);
        sphere2->Transform.SetPositionY(2.4f).SetScale(glm::vec3(1.2f)).SetScaleY(0.3f).SetParent(&parent);
        coneB->Transform.SetPositionY(2.7f).SetScale(glm::vec3(0.8f)).SetScaleZ(0.35f).SetRotationX(90.0f).SetParent(&parent);


        cylinderTop->Transform.SetPositionY(2.3f).SetRotationX(90.f).SetScaleZ(0.2f).SetParent(&parent);
        cylinderMid->Transform.SetPositionY(1.1f).SetRotationX(90.0f).SetScale(glm::vec3(0.8f)).SetScaleZ(1.9f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);

        pieces.push_back(coneT);
        pieces.push_back(sphere1);
        pieces.push_back(sphere2);
        pieces.push_back(coneB);

        pieces.push_back(cylinderTop);
        pieces.push_back(cylinderMid);
        pieces.push_back(cylinderBase);


        return pieces;
    }

    static std::vector<Drawable*> GenerateKnight(const glm::vec3& color, TransformComponent& parent)
    {
        std::vector<Drawable*> pieces;

        const auto sphereL = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto sphereR = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);

        const auto cubeDown = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cubeTop = new Cube(ShaderComponent(m_basicVert, m_basicFrag), color);
        
        
        const auto coneBase = new Cone(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto sphereBase = new Sphere(ShaderComponent(m_basicVert, m_basicFrag), color);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVert, m_basicFrag), color);
        

        sphereL->Transform.SetPosition(glm::vec3(0.3f, 2.7f, 0.4f)).SetScale(glm::vec3(0.2f)).SetParent(&parent);
        sphereR->Transform.SetPosition(glm::vec3(-0.3f, 2.7f, 0.4f)).SetScale(glm::vec3(0.2f)).SetParent(&parent);
        
        cubeDown->Transform.SetPositionY(2.45f).SetPositionZ(-0.25f).SetRotationX(105.0f).SetScaleX(0.6f).SetScaleY(0.4).SetScaleZ(0.9f).SetParent(&parent);
        cubeTop->Transform.SetPositionY(2.45f).SetPositionZ(0.2f).SetRotationX(20.0f).SetScaleX(0.6f).SetScaleY(0.6).SetScaleZ(0.9f).SetParent(&parent);
        coneBase->Transform.SetPositionY(1.6f).SetPositionZ(-0.2f).SetRotationX(75.0f).SetScale(glm::vec3(1.2f)).SetScaleZ(1.8f).SetParent(&parent);
        sphereBase->Transform.SetPositionY(0.6f).SetScale(glm::vec3(1.5f)).SetScaleY(1.3f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);

        pieces.push_back(sphereL);
        pieces.push_back(sphereR);
        
        pieces.push_back(cubeDown);
        pieces.push_back(cubeTop);
        
        pieces.push_back(coneBase);
        pieces.push_back(sphereBase);
        pieces.push_back(cylinderBase);
        return pieces;
    }

    static std::vector<Drawable*> GenerateKnight(const glm::vec3& color, TransformComponent& parent, const TextureComponent& texture)
    {
        std::vector<Drawable*> pieces;

        const auto sphereL = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto sphereR = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);

        const auto cubeDown = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cubeTop = new Cube(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        
        
        const auto coneBase = new Cone(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto sphereBase = new Sphere(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        const auto cylinderBase = new Cylinder(ShaderComponent(m_basicVertTex, m_basicFragTex), color, texture);
        

        sphereL->Transform.SetPosition(glm::vec3(0.3f, 2.7f, 0.4f)).SetScale(glm::vec3(0.2f)).SetParent(&parent);
        sphereR->Transform.SetPosition(glm::vec3(-0.3f, 2.7f, 0.4f)).SetScale(glm::vec3(0.2f)).SetParent(&parent);
        
        cubeDown->Transform.SetPositionY(2.45f).SetPositionZ(-0.25f).SetRotationX(105.0f).SetScaleX(0.6f).SetScaleY(0.4).SetScaleZ(0.9f).SetParent(&parent);
        cubeTop->Transform.SetPositionY(2.45f).SetPositionZ(0.2f).SetRotationX(20.0f).SetScaleX(0.6f).SetScaleY(0.6).SetScaleZ(0.9f).SetParent(&parent);
        coneBase->Transform.SetPositionY(1.6f).SetPositionZ(-0.2f).SetRotationX(75.0f).SetScale(glm::vec3(1.2f)).SetScaleZ(1.8f).SetParent(&parent);
        sphereBase->Transform.SetPositionY(0.6f).SetScale(glm::vec3(1.5f)).SetScaleY(1.3f).SetParent(&parent);
        cylinderBase->Transform.SetRotationX(90.0f).SetScale(glm::vec3(1.5)).SetScaleZ(0.5f).SetParent(&parent);

        pieces.push_back(sphereL);
        pieces.push_back(sphereR);
        
        pieces.push_back(cubeDown);
        pieces.push_back(cubeTop);
        
        pieces.push_back(coneBase);
        pieces.push_back(sphereBase);
        pieces.push_back(cylinderBase);
        return pieces;
    }
    
private:
    inline static const char* m_basicVert =
    #include "../resources/shaders/basic.vert"
        ;
    inline static const char* m_basicFrag =
    #include "../resources/shaders/basic.frag"
        ;
    inline static const char* m_basicVertTex =
    #include "../resources/shaders/basicTex.vert"
        ;
    inline static const char* m_basicFragTex =
    #include "../resources/shaders/basicTex.frag"
        ;
};







