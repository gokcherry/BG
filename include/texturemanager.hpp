
#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include<unordered_map>
#include<string>
namespace graf
{
    using namespace std;
    class TextureManager
    {
    public:
        static void addTextureFromFile(const string& fileName);
        static void activateTexture(const string& fileName);    

    private:
        TextureManager();
        static TextureManager* m_instance;
        static TextureManager* getInstance();
        unordered_map<string,unsigned int>  m_textures;

    };

}