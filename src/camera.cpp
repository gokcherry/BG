#define GLM_ENABLE_EXPERIMENTAL
#include "camera.hpp"
#include<glm/gtx/transform.hpp>
#include "shaderprogram.hpp"
#include "transform.hpp"
namespace graf
{

    Camera::Camera(float fov,float aspect,float near,float far)
    {
        m_type = SceneObjectType::Camera;
        static int cameraCounter = 1;
        m_name = "Camera " + std::to_string(cameraCounter++);
        setPerspective(fov,aspect,near,far);
    }

    void Camera::setPerspective(float fov, float aspect, float near, float far)
    {
        m_fov = fov;
        m_aspect = aspect;
        m_near = near;
        m_far = far;
        projectionMatrix = glm::perspectiveLH(glm::radians(fov),aspect,near,far);
    }
    void Camera::draw(ShaderProgram *program)
    {
        transform->updateRotation();
        transform->update();
        auto combined = transform->parentCombinedMatrix*transform->worldMatrix;
        auto viewMatrix = glm::inverse(combined);
        program->setMat4(UniformNames::Projection,projectionMatrix*viewMatrix);
    }
}
