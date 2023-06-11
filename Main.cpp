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

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#include <random>

size_t movingBallIndex = 0;
glm::vec3 movingBallPosition;
glm::vec3 movingBallDirection;
float movingBallSpeed = 0.15f;


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

void RenderLights() {
    if (ambientLightEnabled) {
        glEnable(GL_LIGHT0);

        GLfloat ambientColor[] = { 5.0f, 5.0f, 5.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
    }
    else {
        glDisable(GL_LIGHT0);
    }

   if (directionalLightEnabled) {
        glEnable(GL_LIGHT1);

        GLfloat lightDirection[] = { 1.0f, 1.0f, -10.0f, 1.0f };
        GLfloat diffuseColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat specularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        glLightfv(GL_LIGHT1, GL_POSITION, lightDirection);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseColor);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specularColor);
    }
    else {
        glDisable(GL_LIGHT1);
    }

    if (pointLightEnabled) {
        glEnable(GL_LIGHT2);

        GLfloat lightPosition[] = { 0.0f, 2.0f, -2.0f, 1.0f};
        GLfloat diffuseColor[] = { 255.0f, 255.0f, 255.0f, 1.0f };
        GLfloat specularColor[] = { 55.0f, 55.0f, 255.0f, 1.0f };
        GLfloat constantAttenuation = 1.0f;
        GLfloat linearAttenuation = 0.0005f;
        GLfloat quadraticAttenuation = 0.001f;

        glLightfv(GL_LIGHT2, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseColor);
        glLightfv(GL_LIGHT2, GL_SPECULAR, specularColor);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, constantAttenuation);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, linearAttenuation);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    }
    else {
        glDisable(GL_LIGHT2);
    }

   if (spotLightEnabled) {
        glEnable(GL_LIGHT3);

        GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat spotDirection[] = { 50.0f, 50.0f, 10.0f };
        GLfloat diffuseColor[] = { 255.0f, 155.0f, 155.0f, 1.0f };
        GLfloat specularColor[] = { 255.0f, 0.0f, 0.0f, 1.0f };
        GLfloat constantAttenuation = 1.0f;
        GLfloat linearAttenuation = 0.0005f;
        GLfloat quadraticAttenuation = 0.00001f;
        GLfloat spotCutoff = 25.0f;
        GLfloat spotExponent = 10.0f;

        glLightfv(GL_LIGHT3, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseColor);
        glLightfv(GL_LIGHT3, GL_SPECULAR, specularColor);
        glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, constantAttenuation);
        glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, linearAttenuation);
        glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, spotCutoff);
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, spotExponent);
    }
    else {
        glDisable(GL_LIGHT3);
    }

}

int main(void)
{
    
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1000, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<glm::vec3> ballPositions;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-20.0f, 20.0f);  // Adjust the range of random values as needed

    std::vector<std::pair<std::vector<Vertex>, Material>> objDataList;
    for (size_t i = 1; i < 16; ++i) {
        objDataList.push_back(Read("PoolBalls/Ball" + std::to_string(i) + ".obj"));
    }
    for (size_t i = 0; i < objDataList.size(); ++i) {
        const float x = dist(gen);
        const float y = dist(gen);
        const float z = dist(gen);
        ballPositions.emplace_back(x, y, 0.0f);

    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the transformation matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(screenWidth / 2, screenHeight / 2, 0);
        glScalef(ZOOM, ZOOM, ZOOM);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 0, 1);

        RenderPoolTable(100.0f, 50.0f, 2.5f, glm::vec3(0.0f, 0.0f, 2.25f));

        if (animate) {
            movingBallIndex = 0;
            movingBallPosition = ballPositions[movingBallIndex];
            movingBallDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f));

            movingBallPosition += movingBallDirection * movingBallSpeed;

            // Verificar colisão com outras bolas
            for (size_t i = 0; i < ballPositions.size(); ++i) {
                if (i != movingBallIndex) {
                    float distance = glm::distance(movingBallPosition, ballPositions[i]);
                    if (distance < 2.0f) {  // Raio das bolas é 1.0f
                        animate = false;
                        break;
                    }
                }
            }

            // Verificar colisão com os limites da mesa
            if (movingBallPosition.x < -48.5f || movingBallPosition.x > 48.5f ||
                movingBallPosition.y < -24.5f || movingBallPosition.y > 24.5f) {
                animate = false;
            }

            // Atualizar a posição da bola na lista de posições das bolas
            ballPositions[movingBallIndex] = movingBallPosition;
        }

        //Render each ball
        for (size_t i = 0; i < objDataList.size(); ++i) {
            const auto& objData = objDataList[i];
            const glm::vec3& startPosition = ballPositions[i];
            RenderBalls(startPosition, glm::vec3(0, 0, 0), objData);
        }
        
        RenderLights();

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


