#pragma once
#include <string>
#include<unordered_map>
#include<glm/glm.hpp>
namespace graf
{
    using namespace std;

    class UniformNames
    {
    public: 
        static string World;
        static string Projection;
    };


    class ShaderProgram
    {
        
    public:
        void create();
        void attachShader(const string& fileName);
        void use();
        void unuse();
        void link();
        void addVariable(const string& varName);
        void setFloat(const string& varName,float value);
        void setMat4(const string& varName,const glm::mat4& value);
        string getErrorMessage(unsigned int shaderId,unsigned int shaderType);
    private:
        unsigned int m_id;
        unordered_map<string,unsigned int> m_variables;
    };
}
