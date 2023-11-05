#include "texture_component.h"
#include "../../utils/gl_call.h"

TextureComponent::TextureComponent(const char* path) : m_id(0), m_width(0), m_height(0), TexturePath(path)
{
    stbi_set_flip_vertically_on_load(true);
    // Load the image file and create a texture
    int channels;
    unsigned char* data = stbi_load(path, &m_width, &m_height, &channels, 0);
    
    if (!data)
    {
        Logger::LogError("Failed to load texture %s", path);
        return;
    }
    
    // Generate a texture
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Set texture parameters (optional)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and set image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureComponent::Bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void TextureComponent::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
