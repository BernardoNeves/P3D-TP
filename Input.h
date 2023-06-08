#include<GL/glew.h>
#include <GLFW/glfw3.h>

extern GLfloat rotationX;
extern GLfloat rotationY;
extern GLfloat ZOOM;

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);