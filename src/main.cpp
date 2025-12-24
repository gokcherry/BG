#define GLM_ENABLE_EXPERIMENTAL
#include "glwindow.hpp"

#include<glm/glm.hpp>
#include "shaderprogram.hpp"
#include "vertexbuffer.hpp"
#include "vertexarrayobject.hpp"
#include "meshloader.hpp"
#include "texturemanager.hpp"
#include "transform.hpp"
#include "shadermanager.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "viewport.hpp"
#include "scene.hpp"
#include <glm/gtx/transform.hpp>
#include<imgui/imgui.h>

using namespace std;
void printMatrix(const glm::mat4& mtx)
{
    if (ImGui::BeginTable("Matrix", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
    {
                
                
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%-.3f",mtx[0][0]);
        ImGui::TableNextColumn(); ImGui::Text("%-.3f",mtx[1][0]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][0]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][0]);
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][1]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][1]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][1]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][1]);
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][2]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][2]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][2]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][2]);
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[0][3]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[1][3]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[2][3]);
        ImGui::TableNextColumn(); ImGui::Text("%.3f",mtx[3][3]);
        ImGui::EndTable();
    }
}
glm::mat4 projectionMatrix(float fov,float aspect,float n,float f)
{
    glm::mat4 mtxProj(0);
    float tanHalfFov = tan(fov/2.0f);
    mtxProj[0][0] = 1.0f/(aspect*tanHalfFov); //parantezi unutma sonuç değişiyor.
    mtxProj[1][1] = 1.0f/tanHalfFov;
    mtxProj[2][2] = (f+n)/(f-n);
    mtxProj[2][3] = 1.0f;
    mtxProj[3][2] = -2*f*n/(f-n);
    return mtxProj;
}

int main(int arc,char** argv)
{
    float width = 1600;
    float height = 900;
    graf::GLWindow window;
    window.create(width,height);



    graf::Model* model = graf::Model::loadModel("Cone_UnlitTextured");
    graf::Model* model2 = graf::Model::loadModel("Circle_UnlitTextured");
    graf::Model* model3 = graf::Model::loadModel("Cube_UnlitTextured");
    graf::Camera camera(60,width/height,0.1f,100.0f);
    graf::Camera camera1(60,width/height,0.1f,100.0f);
    graf::ViewPort* viewport[2];
    graf::Scene scene;
    model->addChild(model2);
    viewport[0] = new graf::ViewPort(0,height/2,width,height/2);
    viewport[1] = new graf::ViewPort(0,0,width,height/2);

    viewport[0]->attachCamera(&camera);
    viewport[1]->attachCamera(&camera1);
    scene.addViewPort(viewport[0]);
    scene.addViewPort(viewport[1]);
    scene.addSceneObject(model);
    scene.addSceneObject(model3);
    float w = 1600.0f;
    float h = 900.0f;
    //---------------------------------------------------------------------------
    //                  KLAVYE FONKSİYONU
    //---------------------------------------------------------------------------    



    window.setKeyboardFunction([&](int key,int action){
        if(key==GLFW_KEY_A) camera.transform->moveLeft();
        if(key==GLFW_KEY_D)camera.transform->moveRight()          ;
        if(key==GLFW_KEY_W) camera.transform->moveForward()       ;
        if(key==GLFW_KEY_S)camera.transform->moveBackward()        ;     

    });
    double oldX;
    double oldY;
    window.mouseCursorPosition(oldX,oldY);
    bool isRightButtonPressed = false;
    window.setMouseButtonFunction([&](int button, int action, int mods ){
        
    
    });



    window.setMouseMoveFunction([&](double x,double y){

        double dx = x-oldX;
        double dy = y-oldY;
        if(window.isMouseRightButtonPressed())
        {
            camera.transform->euler.y+=dx;
            camera.transform->euler.x+=dy;
        }

        oldX = x;
        oldY =y ;
    });

    window.setGuiFunction([&](){
        scene.drawGui();
    
    });


    window.setRenderFunction([&](){
        scene.draw();

    });

    window.begin();
    exit(EXIT_SUCCESS);
}