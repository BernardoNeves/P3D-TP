#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"
#include "Importer.h"
#include <random>


//Global Variables
size_t movingBallIndex = 0;
glm::vec3 movingBallPosition;
glm::vec3 movingBallDirection;
float movingBallSpeed = 0.15f;

/**
 * @brief RenderPoolTable - Renders the Pool Table
 *
 * @param length : The Lenght of the Pool Table
 * @param height : The Height of the Pool Table
 * @param depth : The Depth of the Pool Table
 * @param position : The Position of the Pool Table
 * @return : Void
 */
void RenderPoolTable(float length, float height, float depth, const glm::vec3& position)
{
    // Material properties for the table
    GLfloat tableAmbient[] = { 0.0f, 0.05f, 0.0f, 1.0f };
    GLfloat tableDiffuse[] = { 0.1f, 0.2f, 0.1f, 1.0f };
    GLfloat tableSpecular[] = { 0.04f, 0.1f, 0.04f, 1.0f };
    GLfloat tableShininess = 0.078125f;

    //// Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, tableAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tableDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, tableSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, tableShininess);

    // Calculate half dimensions for convenience
    float halfLength = length / 2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;

    // Render the table as a cube
    glBegin(GL_QUADS);

    // Front face
    glVertex3f(-halfLength, -halfHeight, -halfDepth);
    glVertex3f(-halfLength, halfHeight, -halfDepth);
    glVertex3f(halfLength, halfHeight, -halfDepth);
    glVertex3f(halfLength, -halfHeight, -halfDepth);

    // Back face
    glVertex3f(-halfLength, -halfHeight, halfDepth);
    glVertex3f(-halfLength, halfHeight, halfDepth);
    glVertex3f(halfLength, halfHeight, halfDepth);
    glVertex3f(halfLength, -halfHeight, halfDepth);

    // Left face
    glVertex3f(-halfLength, -halfHeight, -halfDepth);
    glVertex3f(-halfLength, halfHeight, -halfDepth);
    glVertex3f(-halfLength, halfHeight, halfDepth);
    glVertex3f(-halfLength, -halfHeight, halfDepth);

    glVertex3f(halfLength, -halfHeight, -halfDepth);
    glVertex3f(halfLength, halfHeight, -halfDepth);
    glVertex3f(halfLength, halfHeight, halfDepth);
    glVertex3f(halfLength, -halfHeight, halfDepth);

    // Top face
    glVertex3f(-halfLength, halfHeight, -halfDepth);
    glVertex3f(-halfLength, halfHeight, halfDepth);
    glVertex3f(halfLength, halfHeight, halfDepth);
    glVertex3f(halfLength, halfHeight, -halfDepth);

    // Bottom face
    glVertex3f(-halfLength, -halfHeight, -halfDepth);
    glVertex3f(-halfLength, -halfHeight, halfDepth);
    glVertex3f(halfLength, -halfHeight, halfDepth);
    glVertex3f(halfLength, -halfHeight, -halfDepth);

    glEnd();

    // Set the initial position of the table
    glTranslatef(position.x, position.y, position.z);
}

/**
 * @brief DefineLights - Defines the Lights to be Later Used
 */
void DefineLights() {

    //Define the Ambient Light
    GLfloat GL_LIGHT0_ambientColor[] = { 5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, GL_LIGHT0_ambientColor);

    //Define the Directional Light
    GLfloat GL_LIGHT1_lightDirection[] = { 0.0f, 0.0f, 5.0f, 1.0f };
    GLfloat GL_LIGHT1_diffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat GL_LIGHT1_specularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, GL_LIGHT1_lightDirection);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, GL_LIGHT1_diffuseColor);
    glLightfv(GL_LIGHT1, GL_SPECULAR, GL_LIGHT1_specularColor);

    //Define the Point Light
    GLfloat GL_LIGHT2_lightPosition[] = { 0.0f, 2.0f, -2.0f, 1.0f };
    GLfloat GL_LIGHT2_diffuseColor[] = { 255.0f, 255.0f, 255.0f, 1.0f };
    GLfloat GL_LIGHT2_specularColor[] = { 55.0f, 55.0f, 255.0f, 1.0f };
    GLfloat GL_LIGHT2_constantAttenuation = 1.0f;
    GLfloat GL_LIGHT2_linearAttenuation = 0.0005f;
    GLfloat GL_LIGHT2_quadraticAttenuation = 0.001f;

    glLightfv(GL_LIGHT2, GL_POSITION, GL_LIGHT2_lightPosition);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, GL_LIGHT2_diffuseColor);
    glLightfv(GL_LIGHT2, GL_SPECULAR, GL_LIGHT2_specularColor);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, GL_LIGHT2_constantAttenuation);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, GL_LIGHT2_linearAttenuation);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, GL_LIGHT2_quadraticAttenuation);

    //Define the Spot Light
    GLfloat GL_LIGHT3_lightPosition[] = { 50.0f, 50.0f, 2.0f, 1.0f };
    GLfloat GL_LIGHT3_spotDirection[] = { 0.0f, 0.0f, -2.0f };
    GLfloat GL_LIGHT3_diffuseColor[] = { 255.0f, 200.0f, 200.0f, 1.0f };
    GLfloat GL_LIGHT3_specularColor[] = { 255.0f, 100.0f, 100.0f, 1.0f };
    GLfloat GL_LIGHT3_constantAttenuation = 1.0f;
    GLfloat GL_LIGHT3_linearAttenuation = 0.000005f;
    GLfloat GL_LIGHT3_quadraticAttenuation = 0.0000001f;
    GLfloat GL_LIGHT3_spotCutoff = 35.0f;
    GLfloat GL_LIGHT3_spotExponent = 10.0f;

    glLightfv(GL_LIGHT3, GL_POSITION, GL_LIGHT3_lightPosition);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, GL_LIGHT3_spotDirection);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, GL_LIGHT3_diffuseColor);
    glLightfv(GL_LIGHT3, GL_SPECULAR, GL_LIGHT3_specularColor);
    glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, GL_LIGHT3_constantAttenuation);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, GL_LIGHT3_linearAttenuation);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, GL_LIGHT3_quadraticAttenuation);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, GL_LIGHT3_spotCutoff);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, GL_LIGHT3_spotExponent);

}

/**
 * @brief RenderLights - Enable or disable the Lights
 */
void ToggleLights() {

    // Enable or disable the Ambient Light
	ambientLightEnable ? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);

    // Enable or disable the Directional Light 
	directionalLightEnable ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);

	// Enable or disable the Point Light
    pointLightEnable ? glEnable(GL_LIGHT2) : glDisable(GL_LIGHT2);

    // Enable or disable the Spot Light
	spotLightEnable ? glEnable(GL_LIGHT3) : glDisable(GL_LIGHT3);

}

/**
 * @brief WindowSetSpace - Sets the Space to the OpenGL renders
 *
 * @param Window : The window to be used to Set the Space
 * @return : Void
 */
void windowSetSpace(GLFWwindow* window, int* screenWidth, int* screenHeight) {

    //Gets the Screen Width and the Screen Height to be used in future Functions
    int currentWidth, currentHeight;

    //If the Window doens't change returns, else updates our rendering space
    glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
    if (*screenWidth == currentWidth && *screenHeight == currentHeight)
        return;

    *screenWidth = currentWidth;
    *screenHeight = currentHeight;

    //Set the Viewport Parameters, to define the area of the window where OpenGL renders
    glViewport(0, 0, *screenWidth, *screenHeight);

    //Set the Current Matrix Mode, and resets the Current marix to the Identity Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Defines an Orthographic Porjection Matrix
    glOrtho(0, *screenWidth, 0, *screenHeight, -1000, 1000);

	//Multiplies the Current Matrix by a Translation Matrix
    glTranslatef(*screenWidth / 2, *screenHeight / 2, 0);

    //Set the Current Matrix Mode, and resets the Current marix to the Identity Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

/**
 * @brief Main - Program Initialization and Rendering loop
 */
int main(void) {

    int screenWidth = 1280;
    int screenHeight = 720;

    //Initialize the GLFW Library
    if (!glfwInit()) {
        std::cout << "GLFW Initialization Unsucessfull" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Create Variable for the Window
    GLFWwindow* window;
    window = glfwCreateWindow(screenWidth, screenHeight, "Window", NULL, NULL);

    //Checks if the Window was correctly Initiated
    if (!window) {
        std::cout << "Window Initialization Unsucessfull" << std::endl;
        glfwTerminate();
        return -1;
    }

	//Creates the Context of the window
    glfwMakeContextCurrent(window);

    // Initialize the GLEW Library
    if (glewInit() != GLEW_OK) {
		std::cout << "GLEW Initialization Unsucessfull" << std::endl;
		glfwTerminate();
        return -1;
    }

    //Enable the Input System
    InputEnable(window);

    //Gets the Screen Width and the Screen Height to be used in future Functions
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    //Set the Viewport Parameters, to define the area of the window where OpenGL renders
    glViewport(0, 0, screenWidth, screenHeight);

    //Set the Current Matrix Mode, and resets the Current marix to the Identity Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Defines an Orthographic Porjection Matrix
    glOrtho(0, screenWidth, 0, screenHeight, -1000, 1000);

	//Multiplies the Current Matrix by a Translation Matrix
    glTranslatef(screenWidth / 2, screenHeight / 2, 0);

    //Set the Current Matrix Mode, and resets the Current marix to the Identity Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Enables the Depth Test (Does Depth comparisons and update the Depth Buffer), 
                   Lighting (Uses the Current Lighting Paramenters to compute the Vertex color or index), 
                   Blend (Blends the computed fragment color values with the values in the color buffers)*/

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); // Disable when using shaders
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Randomizer Balls' Position
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-20.0f, 20.0f);

    //List of Balls, the Position and the VBOIDs
	std::vector<glm::vec3> ballPositions;


    std::vector<std::pair<std::vector<IMPT::ObjectLoader::Vertex>, IMPT::ObjectLoader::Material>> ObjectDataList = IMPT::ObjectLoader::Read("PoolBalls/");
    std::vector<GLuint> vboIDs = IMPT::ObjectLoader::Send(ObjectDataList);

    for (auto& ObjectData : ObjectDataList){
        const float x = dist(gen) * 2;
        const float y = dist(gen);
        ballPositions.emplace_back(x, y, 0.0f);
    }

 //   //Read Shaders from file
	//std::string vertexShaderCode = readShaderFile("VertexShader.glsl");
	//std::string fragmentShaderCode = readShaderFile("FragmentShader.glsl");

	////Shader Program Initialization
	//GLuint shaderProgram = createShaderProgram(vertexShaderCode, fragmentShaderCode);
    
    //Specify the Clear Values for the Color Buffers
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    //Define the Lights
    DefineLights();

    //Rindering the Loop
	while (!glfwWindowShouldClose(window)) {
       
        //If the Window changes the Shape the Rendering Space
        windowSetSpace(window, &screenWidth, &screenHeight);

        //Clears it to preset Values
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Resets the Current marix to it's Identity Matrix
	    glLoadIdentity();

        //Multiplies the Current Matrix by a General Scaling Matrix and two Rotations Matrices
		glScalef(ZOOM, ZOOM, ZOOM);
		glRotatef(rotationX, 1, 0, 0);
		glRotatef(rotationY, 0, 0, 1);

        //Renders the Pool Table
		RenderPoolTable(100.0f, 50.0f, 2.5f, glm::vec3(0.0f, 0.0f, 2.25f));

        //If the user Want's to Change the Position of the Balls
		if (randomizePosition) {

			for (size_t i = 0; i < ObjectDataList.size(); ++i) {
				const float x = dist(gen) * 2;
				const float y = dist(gen);
				ballPositions[i] = glm::vec3(x, y, 0);
			}
			randomizePosition = false;
            animate = false;
		}

        //If the user Want's to Animate the Balls
        if (animate) {

            movingBallIndex = 0;
            movingBallPosition = ballPositions[movingBallIndex];
            movingBallDirection = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));

            movingBallPosition += movingBallDirection * movingBallSpeed;

            //Verify the Collision betweens balls
            for (size_t i = 0; i < ballPositions.size(); ++i) {
                if (i != movingBallIndex) {
                    float distance = glm::distance(movingBallPosition, ballPositions[i]);
                    if (distance < 2.0f) {  //Radius of the Ball 1.0f
                        animate = false;
                        break;
                    }
                }
            }

            // Check the pool table limits 
            if (movingBallPosition.x < -48.5f || movingBallPosition.x > 48.5f ||
                movingBallPosition.y < -24.5f || movingBallPosition.y > 24.5f) {
                animate = false;
            }

            //Update the Position of the Ball in the List
            ballPositions[movingBallIndex] = movingBallPosition;
        }

		//Render each Ball
		for (size_t i = 0; i < ObjectDataList.size(); ++i) {
            IMPT::ObjectLoader::Draw(ballPositions[i], glm::vec3(ballPositions[i].y * 45, ballPositions[i].x * 45, 0), ObjectDataList[i], vboIDs[i]/*, shaderProgram*/);
		}

        //Render the Lights
		ToggleLights();

		// Swap the front and back buffers
		glfwSwapBuffers(window);

		// Poll for events
		glfwPollEvents();

    }

    //Deletes the Vertex Buffer Objects
    for (GLuint vbo : vboIDs)
        glDeleteBuffers(1, &vbo);

    //Destroy the GLFW Window
    glfwDestroyWindow(window);

    //Terminate the GLFW Library
    glfwTerminate();

    return 0;
}


