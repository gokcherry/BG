#include "shadermanager.hpp"
#include "shaderprogram.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace graf
{
    ShaderManager* ShaderManager::m_Instance = nullptr;

    ShaderManager::ShaderManager()
    {

    }
    ShaderManager* ShaderManager::getInstance()
    {
        if(m_Instance==nullptr)
        {
            m_Instance = new ShaderManager();
        }

        return m_Instance;
    }

    void ShaderManager::addShaderFromFile(const std::string& programName)
    {
        auto manager= getInstance();

        if(manager->m_shaders.count(programName)==0)
        {
            using namespace std;

            string fileName = "./shaders/"+programName+".json";

            ifstream file(fileName);

            using namespace nlohmann;
            auto jsondata = json::parse(file);

            string vertexShaderFileName = jsondata["VertexShader"];
            string fragmentShaderFileName = jsondata["FragmentShader"];

            string vertexShaderPath = "./shaders/"+programName+"/"+vertexShaderFileName;
            string fragmentShaderPath = "./shaders/"+programName+"/"+fragmentShaderFileName;


            ShaderProgram* program = new ShaderProgram();
            program->create();
            program->attachShader(vertexShaderPath);
            program->attachShader(fragmentShaderPath);
            program->link();

            auto uniforms = jsondata["Uniforms"];

            for(auto next:uniforms)
            {
                program->addVariable(next["Name"]);
            }

            manager->m_shaders[programName] = program;

        }//end of if
    }//endof function

    ShaderProgram* ShaderManager::getProgram(const std::string& programName)
    {
        auto manager= getInstance();
        if(manager->m_shaders.count(programName))
        {
            return manager->m_shaders[programName];
        }

        return nullptr;
    }
}