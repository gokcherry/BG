#include "scene.hpp"
#include "model.hpp"
#include "shadermanager.hpp"
#include "shaderprogram.hpp"
#include "transform.hpp"
#include<stack>
#include <imgui/imgui.h>
namespace graf
{
    int id = 1000;
    using namespace std;
    Scene::Scene()
    {
        m_root = new SceneObject();
        m_root->m_name = "Root";
    }
    void Scene::drawScene(ViewPort* viewPort)
    {
        
       
        
        stack<SceneObject*> objects;
        objects.push(m_root);
        while(objects.empty()!=true)
        {
                auto next = objects.top();
                next->transform->updateRotation();
                next->transform->update();
                next->updateChilds();
                objects.pop();
                if(next->getType()==SceneObjectType::Model)
                {
                    Model* model = (Model*)next;
                    string shaderName = model->getShaderName();
                    string textureName = model->getTextureName();
                    ShaderManager::getProgram(shaderName)->use();
                    viewPort->actiaveCamera(ShaderManager::getProgram(shaderName));
                    model->draw(ShaderManager::getProgram(shaderName));
                    ShaderManager::getProgram(shaderName)->unuse();
                }
                for(auto nextChild:next->m_childs )
                {
                    objects.push(nextChild);
                }
        }


    }

    void Scene::drawGui()
    {
        id = 1000;
        ImGui::Begin("Scene");

            drawNextGuiNode(m_root);

        ImGui::End();


        ImGui::Begin("Properties");

            ImGui::PushID(id++);
            if(ImGui::CollapsingHeader("Transformation[0]"))
            {
                ImGui::SliderFloat3("Position",&m_root->transform->position.x,-15.0f,15.0f,"%.1f");
                ImGui::SliderFloat3("Rotation",&m_root->transform->euler.x,-360.0f,360.0f,"%.1f");
                ImGui::SliderFloat3("Scale",&m_root->transform->scale.x,0.1f,5.0f,"%.1f");
            }
            ImGui::PopID();
        ImGui::End();         
    }

    void Scene::addModelFromFile(const string &fileName)
    {
        Model* model = Model::loadModel(fileName);

        addSceneObject(model);
    }
    void Scene::addSceneObject(SceneObject *object)
    {
        m_root->addChild(object);
    }

    void Scene::addViewPort(ViewPort * viewPort)
    {
        m_viewPortList.push_back(viewPort);
    }
    void Scene::draw()
    {
        for(auto nextViewPort:m_viewPortList)
        {
            nextViewPort->activateScissor();
            nextViewPort->clearBackground(0.0f, 0.4f, 0.7f, 1.0f);  
            nextViewPort->activate();  
            
            drawScene(nextViewPort);
        }
    }
    void Scene::drawNextGuiNode(SceneObject *current)
    {
         ImGui::PushID(id++);
        if(ImGui::TreeNode(current->getName().c_str()))
        {
            for(auto nextChild:current->m_childs)
            {
                drawNextGuiNode(nextChild);
            }
            ImGui::TreePop();
        } 
        ImGui::PopID();       
    }
}