#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    std::string textureFile;
    GLuint textureID;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

void LoadTextures(std::pair<std::vector<Vertex>, Material>& objDataList)
{
        std::pair<std::vector<Vertex>, Material>& objData = objDataList;
        Material& material = objData.second;
        // Load texture image
        int width, height, channels;
        unsigned char* image = stbi_load(material.textureFile.c_str(), &width, &height, &channels, 0);
        if (!image) {
            std::cerr << "Failed to load texture image: " << material.textureFile << std::endl;
            return;
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
        material.textureID = textureID;

        // Free the image data
        stbi_image_free(image);
}

std::pair<std::vector<Vertex>, Material> Read(const std::string& obj_model_filepath) {
    std::vector<Vertex> vertices;
    Material material;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    std::string mtlFileName;

    std::ifstream objFile(obj_model_filepath);
    if (!objFile) {
        std::cerr << "Failed to open OBJ file: " << obj_model_filepath << std::endl;
        return std::make_pair(vertices, material);
    }

    std::string line;
    while (std::getline(objFile, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vt") {
            glm::vec2 texcoord;
            iss >> texcoord.x >> texcoord.y;
            texcoords.push_back(texcoord);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "mtllib") {
            iss >> mtlFileName;
        }
        else if (prefix == "f") {
            std::string faceToken;
            while (iss >> faceToken) {
                std::replace(faceToken.begin(), faceToken.end(), '/', ' ');
                std::istringstream faceIss(faceToken);
                int vertexIndex, texcoordIndex, normalIndex;
                faceIss >> vertexIndex >> texcoordIndex >> normalIndex;

                Vertex vertex;
                vertex.position = positions[vertexIndex - 1];
                vertex.texcoord = texcoords[texcoordIndex - 1];
                vertex.normal = normals[normalIndex - 1];
                vertices.push_back(vertex);
            }
        }
    }

    // Load materials from MTL file
    std::string mtlPath = obj_model_filepath.substr(0, obj_model_filepath.find_last_of('/')) + "/" + mtlFileName;
    std::ifstream mtlFile(mtlPath);
    if (mtlFile) {
        //Material material;
        std::string mtlLine;
        while (std::getline(mtlFile, mtlLine)) {
            std::istringstream mtlIss(mtlLine);
            std::string mtlPrefix;
            mtlIss >> mtlPrefix;

            if (mtlPrefix == "newmtl") {
                mtlIss >> material.name;
            }
            else if (mtlPrefix == "Ka") {
                mtlIss >> material.ambient.r >> material.ambient.g >> material.ambient.b;
            }
            else if (mtlPrefix == "Kd") {
                mtlIss >> material.diffuse.r >> material.diffuse.g >> material.diffuse.b;
            }
            else if (mtlPrefix == "Ks") {
                mtlIss >> material.specular.r >> material.specular.g >> material.specular.b;
            }
            else if (mtlPrefix == "Ns") {
                mtlIss >> material.shininess;
            }
            else if (mtlPrefix == "map_Kd") {
                mtlIss >> material.textureFile;
                material.textureFile = obj_model_filepath.substr(0, obj_model_filepath.find_last_of('/')) + "/" + material.textureFile;
            }
        }
    }
    else {
        std::cerr << "Failed to open MTL file: " << mtlPath << std::endl;
    }

    // Load textures
    std::pair<std::vector<Vertex>, Material> objDataList;
    objDataList = (std::make_pair(vertices, material));
    LoadTextures(objDataList);

    return objDataList;
}


GLuint CreateVertexBuffer(const std::vector<Vertex>& vertices) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

void BindVertexArray(const GLuint vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, position)));
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, normal)));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, texcoord)));
}

void RenderBalls(const glm::vec3& position, const glm::vec3& orientation, const std::pair<std::vector<Vertex>, Material>& objData) {
    const std::vector<Vertex>& vertices = objData.first;
    const Material& material = objData.second;

    // Create and bind the vertex buffer object
    GLuint vbo = CreateVertexBuffer(vertices);
    BindVertexArray(vbo);

    // Set the position and orientation of the object
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(orientation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(orientation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(orientation.z, 0.0f, 0.0f, 1.0f);

    // Bind the texture to the object
    glBindTexture(GL_TEXTURE_2D, material.textureID);
    glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(glm::vec4(material.ambient, 1.0f)));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(glm::vec4(material.diffuse, 1.0f)));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(glm::vec4(material.specular, 1.0f)));
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

    // Render the object using the vertex array object
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    // Cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDeleteBuffers(1, &vbo);

    glPopMatrix();
}



