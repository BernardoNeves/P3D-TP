#include"Input.h"
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dist(0.0f, 0.9f);

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat ZOOM = 10.0f;
float xPosBuffer;
float yPosBuffer;

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{

    const GLfloat rotationSpeed = 0.5f;

    if (xPos > xPosBuffer) {
        rotationX -= rotationSpeed;
    }
    if (xPos < xPosBuffer) {
        rotationX += rotationSpeed;
    }
    if (yPos > yPosBuffer) {
        rotationY -= rotationSpeed;
    }
    if (yPos < yPosBuffer) {
        rotationY += rotationSpeed;
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
    //std::cout << key << std::endl;

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
        case GLFW_KEY_BACKSPACE:
            glClearColor(dist(gen), dist(gen), dist(gen), 1.0f);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, 1);
            break;
        }
    }
}