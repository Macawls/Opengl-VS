#define GLEW_STATIC 
#include <GL/glew.h>
#include "shader_component.h"


ShaderComponent::ShaderComponent(const char* vertSource, const char* fragsSource)
{
    vert = compileShader(GL_VERTEX_SHADER, vertSource);
    frag = compileShader(GL_FRAGMENT_SHADER, fragsSource);
    
    ID = createShaderProgram(2, vert, frag);
}

ShaderComponent& ShaderComponent::Use()
{ 
    glUseProgram(ID);
    return *this; 
}

ShaderComponent& ShaderComponent::SetVertexSource(const char* vertSource)
{
    vert = compileShader(GL_VERTEX_SHADER, vertSource);
    return *this;
}

ShaderComponent& ShaderComponent::SetFragmentSource(const char* fragsSource)
{
    frag = compileShader(GL_FRAGMENT_SHADER, fragsSource);
    return *this;
}

ShaderComponent& ShaderComponent::Compile()
{
    ID = createShaderProgram(2, vert, frag);
    return *this;
}

ShaderComponent& ShaderComponent::SetBool(const std::string &name, bool value)
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    return *this;
}

ShaderComponent& ShaderComponent::SetInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    return *this;
}

ShaderComponent& ShaderComponent::SetFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    return *this; 
}

ShaderComponent& ShaderComponent::SetVec2(const std::string &name, const glm::vec2 &value)
{ 
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    return *this; 
}

ShaderComponent& ShaderComponent::SetVec3(const std::string &name, const glm::vec3 &value)
{ 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    return *this; 
}

ShaderComponent& ShaderComponent::SetVec4(const std::string &name, const glm::vec4 &value)
{ 
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    return *this; 
}

ShaderComponent& ShaderComponent::SetMat2(const std::string &name, const glm::mat2 &mat)
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this; 
}

ShaderComponent& ShaderComponent::SetMat3(const std::string &name, const glm::mat3 &mat)
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this; 
}

ShaderComponent& ShaderComponent::SetMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this; 
}