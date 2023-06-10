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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#include <random>


void RenderObject(const std::vector<Vertex>& object, const std::vector<Material>& materials)
{
    // Iterate over the vertices of the object
    glBegin(GL_TRIANGLES);
    for (const Vertex& vertex : object) {
        // Set the position of the vertex
        glPushMatrix();

        // Set the color of the vertex
        glColor3f(1, 1, 1);

        // Set the normal of the vertex
        glNormal3f(vertex.normal.x, vertex.normal.y, vertex.normal.z);

        // Set the texture coordinates of the vertex
        glTexCoord2f(vertex.texcoord.x, vertex.texcoord.y);

        // Draw the vertex
        glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);

        glPopMatrix();
    }
    glEnd();
}



int main(void)
{
    std::vector<std::pair<std::vector<Vertex>, std::vector<Material>>> objDataList;
    for (size_t i = 1; i < 16; ++i) {
        objDataList.push_back(loadOBJ("PoolBalls/Ball" + std::to_string(i) + ".obj"));
    }
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
    glOrtho(0, screenWidth, 0, screenHeight, 0, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<glm::vec3> ballPositions;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);  // Adjust the range of random values as needed

    for (size_t i = 0; i < objDataList.size(); ++i) {
        const float x = dist(gen);
        const float y = dist(gen);
        const float z = dist(gen);
        ballPositions.emplace_back(x, y, 0.0f);

        /

    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glTranslatef(screenWidth / 2, screenHeight / 2, 0); // Move the object to the center of the screen
        glScalef(ZOOM, ZOOM, ZOOM); // Apply zoom
        //glRotatef(rotationX, 0, 1, 0);
        //glRotatef(rotationY, 1, 0, 0);

        

        for (size_t i = 0; i < objDataList.size(); ++i) {
            const auto& objData = objDataList[i];
            const glm::vec3& startPosition = ballPositions[i];
            glTranslatef(startPosition.x, startPosition.y, startPosition.z);
            for (const Material& material : objData.second) {
                // Load texture image
                int width, height, channels;
                unsigned char* image = stbi_load(material.textureFile.c_str(), &width, &height, &channels, 0);
                if (!image) {
                    std::cerr << "Failed to load texture image: " << material.textureFile << std::endl;
                    continue;
                }

                // Generate texture ID
                GLuint textureID;
                glGenTextures(1, &textureID);

                // Bind the texture
                glBindTexture(GL_TEXTURE_2D, textureID);

                // Set the texture parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                // Load the texture image data
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

                // Enable texture mapping
                glEnable(GL_TEXTURE_2D);

                // Bind the texture to the object
                glBindTexture(GL_TEXTURE_2D, textureID);

                // Render the object with the texture
                RenderObject(objData.first, objData.second);

                // Free the image data
                stbi_image_free(image);
            }
        }
        glPopMatrix();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
