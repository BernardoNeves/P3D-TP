#include"Input.h"
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

//Global Variables
GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat ZOOM = 5.0f;
float xPosBuffer;
float yPosBuffer;

bool animate = false;
bool randomizePosition = false;

bool ambientLightEnable = true;
bool directionalLightEnable = true;
bool pointLightEnable = true;
bool spotLightEnable = true;

/**
 * @brief CursorPositionCallback - Checks were the mouse is compared to before, and if it changes, Rotates the View
 *
 * @param Window : Program's Window
 * @param xPos : Position of the Mouse on the X-Axis
 * @param yPos : Position of the Mouse on the Y-Axis
 * @return : Void
 */
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {

    //The Rotation Speed that will be aplicated in the Rotation
    const GLfloat rotationSpeed = 1.0f;

    //Checks the Position of the Mouse Compared to the Buffer Position
    if (xPos > xPosBuffer) {
        rotationY += rotationSpeed;
    }
    else if (xPos < xPosBuffer) {
        rotationY -= rotationSpeed;
    }
    if (yPos > yPosBuffer) {
        rotationX += rotationSpeed;
    }
    else if (yPos < yPosBuffer) {
        rotationX -= rotationSpeed;
    }

    //Changes the Buffer to the Current Position
    xPosBuffer = xPos;
    yPosBuffer = yPos;
}

/**
 * @brief ScrollCallback - Checks if the Scroll, and Zooms if Used
 *
 * @param Window : Program's Window
 * @param xOffSet : The Scroll is moved in the X-Axis
 * @param yOffSet : The Scroll is moved in the Y-Axis
 * @return : Void
 */
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    //Changes the Zoom if the yOffSet Changes 
    ZOOM += fabs(ZOOM) * yOffset * 0.1f;
}

/**
 * @brief KeyCallBack - Checks if Keys are Pressed 
 *
 * @param Window : Program's Window
 * @param Key : The Key that needs to be Checked
 * @param ScanCode : The System-Specific Scancode of the Key
 * @param Action : The Action applied to the Key
 * @param Mods : Which Modifier Keys were Held Down
 * @return : Void
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//The Rotation Speed that will be aplicated in the Rotation
    const GLfloat rotationSpeed = 5;

    /*Actions are GLFW_PRESS (If the Key is Pressed), 
                  GLFW_RELEASE (If the Key is Released), 
                  GLFW_REPEAT (If the Key is being Repeated)*/
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            //If the Key Pressed is Up, the View is Rotated in the X-Axis
            rotationX -= rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
			//If the Key Pressed is Down, the View is Rotated in the X-Axis
            rotationX += rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            //If the Key Pressed is Right, the View is Rotated in the Y-Axis
            rotationY += rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
			//If the Key Pressed is Left, the View is Rotated in the Y-Axis
            rotationY -= rotationSpeed;
            break;
        case GLFW_KEY_TAB:
			//If the Key Pressed is Tab, the Position of the Balls is Changed
            randomizePosition = true;
            break;
        case GLFW_KEY_SPACE:
			//If the Key Pressed is Space, the Animation Starts
            animate = true;
            break;
        case GLFW_KEY_ESCAPE:
            //If the Key Pressed is Escape the Window is Closed
            glfwSetWindowShouldClose(window, 1);
            break;
        case GLFW_KEY_1:
			//If the Key Pressed is One, The Light is Changed to Ambient Light
            ambientLightEnable = !ambientLightEnable;
            break;
        case GLFW_KEY_2:
			//If the Key Pressed is Two, The Light is Changed to Directional Light
            directionalLightEnable = !directionalLightEnable;
            break;
        case GLFW_KEY_3:
			//If the Key Pressed is Three, The Light is Changed to Point Light
            pointLightEnable = !pointLightEnable;
            break;
        case GLFW_KEY_4:
			//If the Key Pressed is Four, The Light is Changed to Spot Light
            spotLightEnable = !spotLightEnable;
            break;
        }
    }
}

/**
 * @brief InputEnable - Input Enable the Inputs Needed in the Program
 *
 * @param Window : Program's Window
 * @return : Void
 */
void InputEnable(GLFWwindow* window) {

    //Sets the Position of the Cursor
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    
    //Sets the Key CallBack
    glfwSetKeyCallback(window, keyCallback);
    
    //Sets an Input Option for the Specified Window
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    
    //Sets the Scroll CallBack
    glfwSetScrollCallback(window, scrollCallback);
}