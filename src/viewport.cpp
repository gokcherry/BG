#include "viewport.hpp"
#include "camera.hpp"
#include "shaderprogram.hpp"
#include<glad/glad.h>
namespace graf
{
    ViewPort::ViewPort(int x, int y, int width, int height)
    {
        this->x=x;
        this->y = y;
        this->width = width;
        this->height = height;

    }
    void ViewPort::activate()
    {
        glViewport(x,y,width,height);
        
    }
    void ViewPort::actiaveCamera(ShaderProgram *program)
    {
        m_camera->draw(program);
    }
    void ViewPort::activateScissor()
    {
        glScissor(x,y,width,height);
    }
    void ViewPort::clearBackground(float r,float g,float b,float a)
    {
        glClearColor(r,g,b,a);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    void ViewPort::attachCamera(Camera *camera)
    {
        camera->setPerspective(camera->m_fov,width/height,camera->m_near,camera->m_far);

        m_camera= camera;

    }
    Camera *ViewPort::getCamera()
    {
        return m_camera;
    }
}