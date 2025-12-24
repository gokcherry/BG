#pragma once
#include<unordered_map>
#include<string>

namespace graf
{
    class ShaderProgram;

    using ShaderMap = std::unordered_map<std::string,ShaderProgram*>;
    
    class ShaderManager
    {
    public:

        static void addShaderFromFile(const std::string& fileName);
        static ShaderProgram* getProgram(const std::string& programName);

    private:

        ShaderManager();

        static ShaderManager* m_Instance;
        static ShaderManager* getInstance();

        ShaderMap m_shaders;
    };
}