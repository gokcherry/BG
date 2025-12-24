#pragma once 
#include<glm/glm.hpp>
#include<vector>
namespace graf
{
    class Camera;
    class ShaderProgram;
    class ViewPort
    {
    public:
        ViewPort(int x,int y,int width,int height);

        void activate();
        void actiaveCamera(ShaderProgram* program);
        void activateScissor();
        void clearBackground(float r,float g,float b,float a);
        void attachCamera(Camera * camera);
        Camera* getCamera();
        int x;
        int y;
        int width;
        int height;
    private:
        Camera* m_camera;
    
    };
    using ViewPortList = std::vector<ViewPort*>;

}