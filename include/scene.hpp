#pragma once
#include "sceneobject.hpp"
#include "viewport.hpp"
namespace graf
{

    class Model;
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
        void drawNextGuiNode(SceneObject* current);
        ViewPortList m_viewPortList;
        SceneObject* m_root;
    };

}