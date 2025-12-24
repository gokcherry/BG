#pragma once
#include "sceneobject.hpp"
#include "viewport.hpp"
namespace graf
{

    class Model;
    class Camera;
    class Scene
    {
    public:
        Scene();

        void drawScene(ViewPort* viewPort);
      
        void drawGui();
        void addModelFromFile(const string& fileName);
        void addSceneObject(SceneObject* object);
        void addViewPort(ViewPort*);
        void draw();
    private:
        SceneObject* createModelNode(const string& fileName);
        Camera* createCameraNode();
        void drawTransformationGui(SceneObject* target,int idSuffix);
        void drawAddModelGui();
        void drawViewportGui();
        void drawNextGuiNode(SceneObject* current);
        ViewPortList m_viewPortList;
        SceneObject* m_root;
        SceneObject* m_selected;
    };

}
