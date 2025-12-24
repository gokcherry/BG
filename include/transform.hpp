#pragma once
#include<glm/glm.hpp>
namespace graf
{
    class Transform
    {
    public:
   
        glm::vec3   position;
        glm::vec3   euler;
        glm::vec3   scale;

        glm::vec3   forward;
        glm::vec3   right;
        glm::vec3   up;
        glm::mat4   worldMatrix;
        glm::mat4   rotationMatrix;
        glm::mat4   parentCombinedMatrix;
        void moveForward(float length=0.1f);
        void moveBackward(float length=0.1f);
        void moveLeft(float length=0.1f);
        void moveRight(float length=0.1f);
        Transform(); 
        void update();

        
        void updateRotation();
        glm::mat4 inverseTransform();
        bool isInstrinsic;
        
    };
}