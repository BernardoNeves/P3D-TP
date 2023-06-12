#include<GL/glew.h>
#include <GLFW/glfw3.h>

//Variables that are Changed in Input.cpp and Used in Main.cpp
extern GLfloat rotationX;
extern GLfloat rotationY;
extern GLfloat ZOOM;
extern bool animate;
extern bool randomizePosition;
extern bool ambientLightEnable;
extern bool directionalLightEnable;
extern bool pointLightEnable;
extern bool spotLightEnable;

//Functions Used in Iput.cpp
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void InputEnable(GLFWwindow* window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);