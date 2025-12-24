#pragma once

#include "sceneobject.hpp"
#include <glm/glm.hpp>
namespace graf
{
    class ShaderProgram;
    class Camera:public SceneObject
    {
    public:
        Camera(float fov=60.0f,float aspect=1.0f,float near=1.0f,float far=100.0f);

        void setPerspective(float fov=60.0f,float aspect=1.0f,float near=1.0f,float far=100.0f);


        void draw(ShaderProgram* program);

        glm::mat4 projectionMatrix;
    private:
        friend class ViewPort;
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
    };

}