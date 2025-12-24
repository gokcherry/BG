#include "shaderfile.hpp"
#include <fstream>
#include <iostream>
#include "glad/glad.h"
namespace graf
{
    using namespace std;
    ShaderFile::ShaderFile(const string& fileName)
    {
        ifstream file(fileName);

        string typeString;
        
        if(file.is_open())
        {
            file>>typeString;

            char readChar;

            while((readChar=file.get())!=EOF)
            {
                m_shaderCode+=readChar;
            }
        
            file.close();

        
        }
        else{
            cout<<fileName<<":dosya acilamadi"<<endl;
        }

        if(typeString=="VertexShader")
        {
            m_type = GL_VERTEX_SHADER;
        }
        else if(typeString=="FragmentShader")
        {
            m_type=GL_FRAGMENT_SHADER;
        }
    }

    string ShaderFile::getSource()
    {
        return m_shaderCode;
    }
    unsigned int ShaderFile::getType()
    {
        return m_type;
    }
}