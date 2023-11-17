#pragma once
#include <vector>
#include "../rendering/vertex.h"
#include "../rendering/texture_component.h"
#include "../rendering/transform.h"
#include "../rendering/lights/directional.h"
#include "../rendering/lights/point.h"
#include "../rendering/lights/spot.h"
#include "../shader/shader_component.h"



class Mesh
{
public:
    // mesh data
    std::vector<Vertex>       Vertices;
    std::vector<unsigned int> Indices;
    TextureComponent Texture;
    ShaderComponent Shader;
    TransformComponent Transform;

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const TextureComponent& texture, const ShaderComponent& shader):
    Vertices(vertices),
    Indices(indices),
    Texture(texture),
    Shader(shader)
    {
        // setup mesh
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
  
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                     &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const ShaderComponent& shader): 
    Vertices(vertices),
    Indices(indices),
     Shader(shader)
    {
    
    
    }
    
    
    
    
    void Draw(
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& color,
        const glm::vec3& camPosition,
        const glm::vec3& camFront,
        const DirectionalLight& dirLight, 
        const std::vector<PointLight>& pointLights,
        const SpotLight& spotLight)
    {
        // Shader
        Shader.Use()
        .SetMat4("model", Transform.GetModelMatrix())
        .SetMat4("view", view)
        .SetMat4("projection", projection)
        .SetVec3("viewPosition", camPosition)
        .SetVec3("color", color)
        .SetVec3("directionalLight.direction", dirLight.Direction)
        .SetVec3("directionalLight.ambient", dirLight.Ambient)
        .SetVec3("directionalLight.diffuse", dirLight.Diffuse)
        .SetVec3("directionalLight.specular", dirLight.Specular)
        .SetVec3("spotLight.position", spotLight.Position)
        .SetVec3("spotLight.direction", camFront)
        .SetVec3("spotLight.ambient", spotLight.Ambient)
        .SetVec3("spotLight.diffuse", spotLight.Diffuse)
        .SetVec3("spotLight.specular", spotLight.Specular)
        .SetFloat("spotLight.constant", spotLight.Constant)
        .SetFloat("spotLight.linear",  spotLight.Linear)
        .SetFloat("spotLight.quadratic", spotLight.Quadratic)
        .SetFloat("spotLight.cutOff", spotLight.CutOff)
        .SetFloat("spotLight.outerCutOff", spotLight.OuterCutOff);

        for (size_t i = 0; i < pointLights.size(); i++)
        {
            std::string index = std::to_string(i);
            Shader
            .SetVec3("pointLights[" + index + "].position", pointLights[i].Transform.Position)
            .SetVec3("pointLights[" + index + "].ambient",pointLights[i].Ambient)
            .SetVec3("pointLights[" + index + "].diffuse", pointLights[i].Diffuse)
            .SetVec3("pointLights[" + index + "].specular", pointLights[i].Specular)
            .SetFloat("pointLights[" + index + "].constant", pointLights[i].Constant)
            .SetFloat("pointLights[" + index + "].linear", pointLights[i].Linear)
            .SetFloat("pointLights[" + index + "].quadratic", pointLights[i].Quadratic);
        }

        if (hasTexture)
        {
            //Texture
            Texture.Bind(0);
            Shader.SetInt("texture1", 0);
        }
        // Draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    }
    
private:
    bool hasTexture;
    //  render data
    unsigned int VAO, VBO, EBO;
}; 
