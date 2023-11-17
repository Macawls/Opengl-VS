#pragma once
#include "../shader/shader_component.h"

/*

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "mesh.h"

class Model
{
public:
    std::vector<Mesh> Meshes;
    ShaderComponent Shader;

    Model(const char* path, const ShaderComponent& shader) : Shader(shader)
    {
        loadModel(path);
    }

    void Draw(const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& color,
        const glm::vec3& camPosition,
        const glm::vec3& camFront,
        const DirectionalLight& dirLight,
        const std::vector<PointLight>& pointLights,
        const SpotLight& spotLight)
    {
        for(unsigned int i = 0; i < Meshes.size(); i++)
        {
            Meshes[i].Draw(view, projection, color, camPosition, camFront, dirLight, pointLights, spotLight);
        }
    }

private:
    void loadModel(const std::string& path)
    {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
	
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            Logger::LogError("Assimp Error %s", import.GetErrorString());
            return;
        }
        
        auto directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    
    void processNode(const aiNode *node, const aiScene *scene)
    {
        // process node's meshes
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            Meshes.push_back(processMesh(mesh, scene));			
        }

        // children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    
    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Vertex
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

            // Vertices
            glm::vec3 vector; 
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z; 
            vertex.Position = vector;

            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            // Texture
            if(mesh->mTextureCoords[0]) // hasTexture
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else // noTextures
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);  
            }
            
            vertices.push_back(vertex);
        }

        // Indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        } 
        
        // process material
        
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                                aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material, 
                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        

        return Mesh(vertices, indices, Shader);
    }  
};
*/