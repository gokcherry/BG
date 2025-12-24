
#pragma once

#include <string>
#include <unordered_map>

namespace graf
{
    class TextureManager
    {
    public:
        static void addTextureFromFile(const std::string& fileName);
        static void activateTexture(const std::string& fileName);

    private:
        TextureManager();
        static TextureManager* m_instance;
        static TextureManager* getInstance();
        static unsigned int createFallbackTexture(const std::string& key);
        std::unordered_map<std::string, unsigned int> m_textures;
    };

}
