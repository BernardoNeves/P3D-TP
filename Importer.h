#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    std::string textureFile;
};


struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

std::pair<std::vector<Vertex>, std::vector<Material>> loadOBJ(const std::string& objPath) {
    std::vector<Vertex> vertices;
    std::vector<Material> materials;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    std::string mtlFileName;

    std::ifstream objFile(objPath);
    if (!objFile) {
        std::cerr << "Failed to open OBJ file: " << objPath << std::endl;
        return std::make_pair(vertices, materials);
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
                std::istringstream faceIss(faceToken);
                std::string vertexIndexStr, texcoordIndexStr, normalIndexStr;
                std::getline(faceIss, vertexIndexStr, '/');
                std::getline(faceIss, texcoordIndexStr, '/');
                std::getline(faceIss, normalIndexStr, '/');

                int vertexIndex = std::stoi(vertexIndexStr) - 1;
                int texcoordIndex = std::stoi(texcoordIndexStr) - 1;
                int normalIndex = std::stoi(normalIndexStr) - 1;

                Vertex vertex;
                vertex.position = positions[vertexIndex];
                vertex.texcoord = texcoords[texcoordIndex];
                vertex.normal = normals[normalIndex];

                vertices.push_back(vertex);
            }
        }
    }

    // Load materials from MTL file
    std::string mtlPath = objPath.substr(0, objPath.find_last_of('/')) + "/" + mtlFileName;
    std::ifstream mtlFile(mtlPath);
    if (mtlFile) {
        std::string mtlLine;
        Material material;
        while (std::getline(mtlFile, mtlLine)) {
            std::istringstream mtlIss(mtlLine);
            std::string mtlPrefix;
            mtlIss >> mtlPrefix;

            if (mtlPrefix == "newmtl") {
                if (!material.name.empty()) {
                    materials.push_back(material);
                }
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
                material.textureFile = objPath.substr(0, objPath.find_last_of('/')) + "/" + material.textureFile;
            }
        }
        if (!material.name.empty()) {
            materials.push_back(material);
        }
    }
    else {
        std::cerr << "Failed to open MTL file: " << mtlPath << std::endl;
    }

    return std::make_pair(vertices, materials);
}
