#pragma once
#include "shader_utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderComponent
{
public:
    unsigned int ID = 0;
    unsigned int vert = 0;
    unsigned int frag = 0;

    ShaderComponent() {}
    ShaderComponent(const char* vertSource, const char* fragsSource);

    ShaderComponent& SetVertexSource(const char* vertSource);
    ShaderComponent& SetFragmentSource(const char* fragsSource);
    
    ShaderComponent& Compile();
    
    ShaderComponent& Use();

    // Primitive
    ShaderComponent& SetBool(const std::string &name, bool value);
    ShaderComponent& SetInt(const std::string &name, int value);
    ShaderComponent& SetFloat(const std::string &name, float value);
    
    // Vector
    ShaderComponent& SetVec2(const std::string &name, const glm::vec2 &value);
    ShaderComponent& SetVec3(const std::string &name, const glm::vec3 &value);
    ShaderComponent& SetVec4(const std::string &name, const glm::vec4 &value);
    
    // Matrix
    ShaderComponent& SetMat2(const std::string &name, const glm::mat2 &mat);
    ShaderComponent& SetMat3(const std::string &name, const glm::mat3 &mat);
    ShaderComponent& SetMat4(const std::string &name, const glm::mat4 &mat);
};