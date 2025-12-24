#pragma once
#include <string>
using namespace std;
namespace graf
{
    class ShaderFile
    {
    public:
        ShaderFile(const string& fileName);
        string getSource();
        unsigned int getType();
    private:
        string m_shaderCode;
        unsigned int m_type;
    };
}