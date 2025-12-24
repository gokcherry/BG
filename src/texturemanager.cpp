#include "texturemanager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <iostream>

namespace graf
{
    TextureManager* TextureManager::m_instance = nullptr;

    void TextureManager::addTextureFromFile(const std::string& fileName)
    {
        TextureManager* manager = getInstance();

        if (!manager->m_textures.count(fileName))
        {
            int width, height, channels;
            stbi_set_flip_vertically_on_load(true);

            std::string fullName = "./images/" + fileName;
            unsigned char* data = stbi_load(fullName.c_str(), &width, &height, &channels, 0);

            if (!data)
            {
                std::cerr << "[TextureManager] Failed to load texture '" << fileName
                          << "' at path '" << fullName << "'. Using fallback texture instead.\n";
                manager->m_textures[fileName] = createFallbackTexture(fileName);
                return;
            }

            unsigned int textureId;
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            manager->m_textures[fileName] = textureId;
            stbi_image_free(data);
        }
    }

    void TextureManager::activateTexture(const std::string& fileName)
    {
        TextureManager* manager = getInstance();

        if (manager->m_textures.count(fileName))
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, manager->m_textures[fileName]);
        }
    }

    TextureManager* TextureManager::getInstance()
    {
        if (m_instance == nullptr)
            m_instance = new TextureManager();
        return m_instance;
    }

    TextureManager::TextureManager()
    {
    }

    unsigned int TextureManager::createFallbackTexture(const std::string& key)
    {
        unsigned int textureId = 0;
        const unsigned char checkerData[] = {
            255, 0,   255, 255,   0,   0,   0,   255,
            0,   0,   0,   255,   255, 0,   255, 255
        }; // 2x2 RGBA checkerboard to highlight missing textures

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerData);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cerr << "[TextureManager] Using fallback checker texture for key '" << key << "'.\n";
        return textureId;
    }

}
