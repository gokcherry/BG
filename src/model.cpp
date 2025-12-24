#include "model.hpp"
#include "shaderprogram.hpp"
#include "vertexarrayobject.hpp"
#include "transform.hpp"
#include "texturemanager.hpp"
#include "shadermanager.hpp"
#include "meshloader.hpp"
#include<fstream>
#include <nlohmann/json.hpp>

namespace graf
{
    Model *Model::loadModel(const string &modelFileName)
    {
        Model* model = new Model();

        string fileName = "./models/"+modelFileName+".json";
        model->m_type = SceneObjectType::Model;
        
        ifstream file(fileName);

        using namespace nlohmann;

        auto jsondata = json::parse(file);

        string modelName = jsondata["ModelName"];
        model->m_name = modelName;
        if(jsondata.count("TextureName")>0)
        {
            string textureName = jsondata["TextureName"];
            model->m_textureName = textureName;
            TextureManager::addTextureFromFile(textureName);
        }
        string shaderProgramName = jsondata["ShaderProgramName"];

        model->m_programName = shaderProgramName;
        ShaderManager::addShaderFromFile(shaderProgramName);

        glm::vec3 scale,position,euler;
        
        scale.x = (jsondata["Scale"][0].get<float>());
        scale.y = (jsondata["Scale"][1].get<float>());
        scale.z = (jsondata["Scale"][2].get<float>());

        position.x = (jsondata["Position"][0].get<float>());
        position.y = (jsondata["Position"][1].get<float>());
        position.z = (jsondata["Position"][2].get<float>());

        euler.x = (jsondata["Euler"][0].get<float>());
        euler.y = (jsondata["Euler"][1].get<float>());
        euler.z = (jsondata["Euler"][2].get<float>());

        model->transform->position = position;
        model->transform->euler = euler;
        model->transform->scale = scale;

        bool isBasicShape = jsondata["IsBasicShape"];

        if(isBasicShape)
        {
            auto shapeType = jsondata["ShapeType"];
            model->m_vao = MeshLoader::createMesh(shapeType);
        }
        else
        {
            string meshFileName = jsondata["MeshFile"];
            model->m_vao = MeshLoader::loadMeshFromFile(meshFileName);
        }


        return model;
    }
    void Model::draw(ShaderProgram *program)
    {
        transform->updateRotation();
        transform->update();
        updateChilds();
        program->setMat4(UniformNames::World,transform->parentCombinedMatrix*transform->worldMatrix);
        m_vao->draw();
    }
    string Model::getTextureName() const
    {
        return m_textureName;
    }
    string Model::getShaderName() const
    {
        return m_programName;
    }
}