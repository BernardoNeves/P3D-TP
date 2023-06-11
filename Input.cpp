#include"Input.h"
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>


GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat ZOOM = 5.0f;
float xPosBuffer;
float yPosBuffer;
bool animate = false;

bool ambientLightEnabled = true;
bool directionalLightEnabled = true;
bool pointLightEnabled = true;
bool spotLightEnabled = true;

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{

    const GLfloat rotationSpeed = 1.0f;

    if (xPos > xPosBuffer) {
        rotationY += rotationSpeed;
    }
    if (xPos < xPosBuffer) {
        rotationY -= rotationSpeed;
    }
    if (yPos > yPosBuffer) {
        rotationX += rotationSpeed;
    }
    if (yPos < yPosBuffer) {
        rotationX -= rotationSpeed;
    }


    std::cout << xPos << "  :  " << yPos << std::endl;
    xPosBuffer = xPos;
    yPosBuffer = yPos;
}

void cursorEnterCallback(GLFWwindow* window, int entered)
{
    if (entered) {
        std::cout << "Entered Windowed" << std::endl;
    } else {
        std::cout << "Left Window" << std::endl;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        std::cout << "Right button pressed" << std::endl;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        std::cout << "Right button released" << std::endl;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left button pressed" << std::endl;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        std::cout << "Left button released" << std::endl;
    }

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		std::cout << "Middle button pressed" << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		std::cout << "Middle button released" << std::endl;
	}
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    std::cout << xOffset << "  :  " << yOffset << std::endl;

    if (yOffset == 1) {
        ZOOM += fabs(ZOOM) * 0.1f;
    } else if (yOffset == -1) {
        ZOOM -= fabs(ZOOM) * 0.1f;
    }

    std::cout << "ZOOM = " << ZOOM << std::endl;

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << key << std::endl;

    const GLfloat rotationSpeed = 5;

    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            rotationX -= rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            rotationX += rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            rotationY += rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            rotationY -= rotationSpeed;
            break;
        case GLFW_KEY_SPACE:
            animate = true;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, 1);
            break;
        case GLFW_KEY_1:
            ambientLightEnabled = !ambientLightEnabled;
            break;
        case GLFW_KEY_2:
            directionalLightEnabled = !directionalLightEnabled;
            break;
        case GLFW_KEY_3:
            pointLightEnabled = !pointLightEnabled;
            break;
        case GLFW_KEY_4:
            spotLightEnabled = !spotLightEnabled;
            break;
        }
    }
}