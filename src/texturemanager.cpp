#include "texturemanager.hpp"

#include<stb/stb_image.h>
#include<glad/glad.h>
namespace graf
{
    TextureManager* TextureManager::m_instance=nullptr;

    void TextureManager::addTextureFromFile(const string &fileName)
    {
        TextureManager* manager= getInstance();


        if(!manager->m_textures.count(fileName))
        {
            //Kaplamayi yükle
            int width,height,channels;
            stbi_set_flip_vertically_on_load(true);

            string fullName = "./images/"+fileName;
            unsigned char* data=  stbi_load(fullName.c_str(),&width,&height,&channels,0);
            unsigned int textureId;
            glGenTextures(1,&textureId);
            glBindTexture(GL_TEXTURE_2D,textureId);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
            glGenerateMipmap(GL_TEXTURE_2D);


            manager->m_textures[fileName] = textureId;
            stbi_image_free(data);

        }
    }

    void TextureManager::activateTexture(const string &fileName)
    {
        TextureManager* manager= getInstance();

        if(manager->m_textures.count(fileName))
        {
            glBindTexture(GL_TEXTURE_2D,manager->m_textures[fileName]);
            
        }
    }

    TextureManager *TextureManager::getInstance()
    {
        if(m_instance==nullptr)
            m_instance = new TextureManager();
        return m_instance;
    }
    TextureManager::TextureManager()
    {
        
    }

}