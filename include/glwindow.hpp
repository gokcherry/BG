#ifndef GLWindow_hpp
#define GLWindow_hpp
#define GLFW_INCLUDE_NONE
#include <iostream>
#include<GLFW/glfw3.h>
#include"glad/glad.h"
#include "functiontypes.hpp"
namespace graf
{
    class GLWindow
    {
        public:
            void create(int width,int height);

            GLFWwindow* getWindow();
            void begin();
            void setRenderFunction(RenderFunction function);
            void setGuiFunction(RenderFunction function);
            void setKeyboardFunction(KeyboardFunction function);
            void setMouseMoveFunction(MouseMoveFunction function);
            void setMouseButtonFunction(MouseButtonFunction function);
            friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
            friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            void mouseCursorPosition(double& x,double&y);
            bool isMouseRightButtonPressed();
            
        private:
            void renderImgui();
            void initilizeImgui();
            RenderFunction m_renderFunction;
            RenderFunction m_guiFunction;
            KeyboardFunction m_keyboardFunction;
            MouseMoveFunction m_mouseMoveFunction;
            MouseButtonFunction m_mouseButtonFunction;
            GLFWwindow* m_window;
    };
}


#endif