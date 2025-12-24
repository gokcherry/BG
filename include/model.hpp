#pragma once
#include "sceneobject.hpp"
#include<string>
using namespace std;
namespace graf
{

    class ShaderProgram;
    class VertexArrayObject;
    class Model:public SceneObject
    {
    public:
        static Model* loadModel(const string& modelName);
        void draw(ShaderProgram* program);
        string getTextureName()const;
        string getShaderName() const;
    private:
        VertexArrayObject*  m_vao;
        string m_programName;
        string m_textureName;
    };

}