#include "scene.hpp"
#include "model.hpp"
#include "shadermanager.hpp"
#include "shaderprogram.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include<stack>
#include <vector>
#include <imgui/imgui.h>
#include <glm/gtx/transform.hpp>
namespace graf
{
    int id = 1000;
    using namespace std;
    Scene::Scene()
    {
        m_root = new SceneObject();
        m_root->m_name = "Root";
        m_root->m_type = SceneObjectType::Empty;
        m_selected = m_root;
    }
    void Scene::drawScene(ViewPort* viewPort)
    {
        m_root->transform->parentCombinedMatrix = glm::mat4(1.0f);
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
                    ShaderProgram* program = ShaderManager::getProgram(shaderName);
                    if(program)
                    {
                        program->use();
                        viewPort->actiaveCamera(program);
                        model->draw(program);
                        program->unuse();
                    }
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

            drawAddModelGui();

            if(m_selected)
            {
                ImGui::Separator();
                ImGui::Text("Selected: %s",m_selected->getName().c_str());
                ImGui::PushID(id++);
                drawTransformationGui(m_selected,id);
                ImGui::PopID();

                if(m_selected!=m_root)
                {
                    if(ImGui::Button("Remove Selected"))
                    {
                        if(auto parent = m_selected->getParent())
                        {
                            parent->removeChild(m_selected);
                            m_selected = parent;
                        }
                    }
                }
            }

            ImGui::Separator();
            drawViewportGui();
        ImGui::End();         
    }

    void Scene::addModelFromFile(const string &fileName)
    {
        Model* model = Model::loadModel(fileName);

        addSceneObject(model);
    }
    void Scene::addSceneObject(SceneObject *object)
    {
        SceneObject* parent = m_selected?m_selected:m_root;
        parent->addChild(object);
        m_selected = object;
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
    SceneObject* Scene::createModelNode(const string &fileName)
    {
        Model* model = Model::loadModel(fileName);
        addSceneObject(model);
        return model;
    }
    Camera* Scene::createCameraNode()
    {
        Camera* camera = new Camera();
        camera->transform->position = glm::vec3(0.0f,0.0f,-5.0f);
        addSceneObject(camera);
        return camera;
    }
    void Scene::drawTransformationGui(SceneObject *target,int idSuffix)
    {
        if(ImGui::CollapsingHeader("Transformation"))
        {
            ImGui::SliderFloat3("Position",&target->transform->position.x,-50.0f,50.0f,"%.2f");
            ImGui::InputFloat3("Pos Input",&target->transform->position.x,"%.3f");
            ImGui::SliderFloat3("Rotation",&target->transform->euler.x,-360.0f,360.0f,"%.2f");
            ImGui::InputFloat3("Rot Input",&target->transform->euler.x,"%.3f");
            ImGui::SliderFloat3("Scale",&target->transform->scale.x,0.1f,10.0f,"%.2f");
            ImGui::InputFloat3("Scale Input",&target->transform->scale.x,"%.3f");
        }
    }
    void Scene::drawAddModelGui()
    {
        if(ImGui::CollapsingHeader("Add Model"))
        {
            if(ImGui::Button("Cube"))
            {
                createModelNode("Cube_UnlitTextured");
            }
            ImGui::SameLine();
            if(ImGui::Button("Cylinder"))
            {
                createModelNode("Cylinder_UnlitTextured");
            }
            ImGui::SameLine();
            if(ImGui::Button("Pyramid"))
            {
                createModelNode("Pyramid_UnlitTextured");
            }
            ImGui::SameLine();
            if(ImGui::Button("Ground"))
            {
                createModelNode("Ground_Grass");
            }
            if(ImGui::Button("Wall"))
            {
                createModelNode("Wall_Block");
            }
            if(ImGui::Button("Camera"))
            {
                createCameraNode();
            }
        }
    }
    void Scene::drawViewportGui()
    {
        if(ImGui::CollapsingHeader("Viewports"))
        {
            std::vector<Camera*> cameras;
            std::stack<SceneObject*> objects;
            objects.push(m_root);
            while(!objects.empty())
            {
                auto current = objects.top();
                objects.pop();
                if(current->getType()==SceneObjectType::Camera)
                {
                    cameras.push_back(static_cast<Camera*>(current));
                }
                for(auto child: current->m_childs)
                    objects.push(child);
            }
            for(size_t i=0;i<m_viewPortList.size();++i)
            {
                auto vp = m_viewPortList[i];
                std::string label = "Viewport " + std::to_string(i+1);
                ImGui::Text("%s (%d x %d)",label.c_str(),vp->width,vp->height);
                int currentIndex = -1;
                for(size_t camIndex=0;camIndex<cameras.size();++camIndex)
                {
                    if(vp->getCamera()==cameras[camIndex])
                        currentIndex = static_cast<int>(camIndex);
                }
                std::vector<const char*> cameraNames;
                for(auto cam: cameras)
                    cameraNames.push_back(cam->getName().c_str());
                if(ImGui::BeginCombo((label+" Camera").c_str(), currentIndex>=0?cameraNames[currentIndex]:"<None>"))
                {
                    for(size_t camIndex=0;camIndex<cameras.size();++camIndex)
                    {
                        bool selected = (currentIndex==(int)camIndex);
                        if(ImGui::Selectable(cameraNames[camIndex],selected))
                        {
                            vp->attachCamera(cameras[camIndex]);
                        }
                    }
                    ImGui::EndCombo();
                }
            }
        }
    }
    void Scene::drawNextGuiNode(SceneObject *current)
    {
         ImGui::PushID(id++);
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow|ImGuiTreeNodeFlags_SpanAvailWidth;
        if(current==m_selected)
            flags |= ImGuiTreeNodeFlags_Selected;
        if(current->m_childs.empty())
            flags |= ImGuiTreeNodeFlags_Leaf|ImGuiTreeNodeFlags_NoTreePushOnOpen;

        bool open = ImGui::TreeNodeEx(current->getName().c_str(),flags);
        if(ImGui::IsItemClicked())
        {
            m_selected = current;
        }
        if(open && !(flags & ImGuiTreeNodeFlags_Leaf))
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
