#include"shaderprogram.hpp"
#include "glad/glad.h"
#include "shaderfile.hpp"
#include <iostream>
namespace graf
{

    string UniformNames::World= "uWorldMatrix";
    string UniformNames::Projection = "uProjectionMatrix";

    void ShaderProgram::create()
    {
        m_id = glCreateProgram();
    }
    void ShaderProgram::attachShader(const string& fileName)
    {
        ShaderFile file(fileName);

        unsigned int type = file.getType();
        string source = file.getSource();
        const char *chSource = &(source[0]);
        //shader oluşturuluyor
        unsigned int shaderId = glCreateShader(type);
        glShaderSource(shaderId, 1, &chSource, NULL);
        glCompileShader(shaderId);

        glAttachShader(m_id, shaderId);

        cout<<getErrorMessage(shaderId,type)<<endl;
    }
    void ShaderProgram::link()
    {
        glLinkProgram(m_id);
    }
    void ShaderProgram::addVariable(const string &varName)
    {
        m_variables[varName] =  glGetUniformLocation(m_id,varName.c_str());
    }
    void ShaderProgram::setFloat(const string &varName, float value)
    {
        glUniform1f(m_variables[varName],value);
    }
    void ShaderProgram::setMat4(const string &varName, const glm::mat4 &value)
    {
        glUniformMatrix4fv(m_variables[varName],1,0,&value[0][0]);
    }
    void ShaderProgram::use()
    {
        glUseProgram(m_id);
    }
    void ShaderProgram::unuse()
    {
        glUseProgram(0);
    }

    string ShaderProgram::getErrorMessage(unsigned int shaderId,unsigned int shaderType)
    {
        int isCompiled;
        char log[512];
        string errorMessage;
        glGetShaderiv(shaderId,GL_COMPILE_STATUS,&isCompiled);

        if(!isCompiled)
        {
            glGetShaderInfoLog(shaderId,512,0,log);
            std::string strType;
            switch(shaderType)
            {
            case GL_VERTEX_SHADER:
                strType="Vertex Shader";
                break;
            case GL_FRAGMENT_SHADER:
                strType="Fragment Shader";
                break;      
            }
            errorMessage="Shader Type:"+strType+" "+log;
        }
        return errorMessage;
    }
}