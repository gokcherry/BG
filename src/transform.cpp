#define GLM_ENABLE_EXPERIMENTAL
#include "transform.hpp"
#include<glm/gtx/transform.hpp>

namespace Graf
{

}

void graf::Transform::moveForward(float length)
{
    position+=forward*length;
}

void graf::Transform::moveBackward(float length)
{
    position-=forward*length;
}
void graf::Transform::moveLeft(float length)
{
    position-=right*length;
}

void graf::Transform::moveRight(float length)
{
    position+=right*length;
}
graf::Transform::Transform()
{
    position = glm::vec3(0);
    euler = glm::vec3(0);
    scale = glm::vec3(1);
    worldMatrix = glm::identity<glm::mat4>();
    rotationMatrix = glm::identity<glm::mat4>();
    parentCombinedMatrix = glm::identity<glm::mat4>();
    isInstrinsic = false;
}

void graf::Transform::update()
{
    glm::mat4 mtxTranslate = glm::translate(position);
    glm::mat4 mtxScale = glm::scale(scale);

    worldMatrix = mtxTranslate*rotationMatrix*mtxScale;

}

void graf::Transform::updateRotation()
{
    if(isInstrinsic)
    {
            rotationMatrix = glm::mat4(1);
            float radX =  glm::radians(euler.x);
            float radY =  glm::radians(euler.y);
            float radZ =  glm::radians(euler.z);

            auto vecRight   = glm::vec3(1.0f,0.0f,0.0f);
            auto vecUp      = glm::vec3(0.0f,1.0f,0.0f);
            auto vecLook    = glm::vec3(0.0f,0.0f,1.0f);

            glm::mat4 mtxRotZ = glm::rotate(radZ,vecLook);

            vecRight      = mtxRotZ*glm::vec4(vecRight,0.0f);
            vecUp         = mtxRotZ*glm::vec4(vecUp,0.0f);
        
            if(radY)
            {
                glm::mat4 mtxRotY = glm::rotate(radY,vecUp);

                vecRight    = mtxRotY*glm::vec4(vecRight,0.0f);
                vecLook     = mtxRotY*glm::vec4(vecLook,0.0f);
            }

            if(radX)
            {
                glm::mat4 mtxRotX = glm::rotate(radX,vecRight);

                vecLook    = mtxRotX*glm::vec4(vecLook,0.0f);
                vecUp       = mtxRotX*glm::vec4(vecUp,0.0f);

            }
            vecRight    = glm::normalize(vecRight);
            vecUp       = glm::normalize(vecUp);
            vecLook     = glm::normalize(vecLook);
            
            rotationMatrix[0] = glm::vec4(vecRight,0.0f);
            rotationMatrix[1] = glm::vec4(vecUp,0.0f);
            rotationMatrix[2] = glm::vec4(vecLook,0.0f);
           
    }
    else
    {
        glm::mat4 mtxRotX = glm::rotate(glm::radians(euler.x),glm::vec3(1.0f,0.0f,0.0f));
        glm::mat4 mtxRotY = glm::rotate(glm::radians(euler.y),glm::vec3(0.0f,1.0f,0.0f));
        glm::mat4 mtxRotZ = glm::rotate(glm::radians(euler.z),glm::vec3(0.0f,0.0f,1.0f));

        rotationMatrix = mtxRotZ*mtxRotY*mtxRotX;
    }

    right = rotationMatrix[0];
    up = rotationMatrix[1];
    forward = rotationMatrix[2];
}

glm::mat4 graf::Transform::inverseTransform()
{
    glm::mat4 mtxInverse;

    glm::mat4 inverseTranslate = glm::translate(-position);
    glm::mat4 inverseRotation = glm::inverse(rotationMatrix);

    mtxInverse = inverseRotation*inverseTranslate;

    return mtxInverse;
}
