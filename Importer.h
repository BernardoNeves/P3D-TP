#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace IMPT {
    class ObjectLoader {
    public:

        //Material Struct that will be used to Store the Object Data
        struct Material {
            std::string name;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;
            std::string textureFile;
            GLuint textureID;
        };

        //Vertices Struct will be used to Store the Vertexes of the Object
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec2 texcoord;
            glm::vec3 normal;
        };

        /**
         * @brief LoadTextures - Load Textures Loads the Textures
         *
         * @param ObjectData : The Object Data
         * @return : Void
         */
        static void LoadTextures(std::vector<std::pair<std::vector<Vertex>, Material>>* objDataList) {
            for (auto& objectData : *objDataList) {
                //The Material of the Object Data
                Material& material = objectData.second;

                //Loads the Texture Image
                int width, height, channels;
                unsigned char* image = stbi_load(material.textureFile.c_str(), &width, &height, &channels, 0);

                //Check's if there were an error in the Loading of the Texture Image
                if (!image) {
                    std::cerr << "Failed to load texture image: " << material.textureFile << std::endl;
                    return;
                }

                //Generate the Texture ID
                GLuint textureID;
                glGenTextures(1, &textureID);

                //Activate the desired texture unit
                //glActiveTexture(GL_TEXTURE0 + textureID);

                //Bind The Texture
                glBindTexture(GL_TEXTURE_2D, textureID);

                //Set The Texture Parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                //Loads the Texture Image Data
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

                //Enables the Texture Mapping
                glEnable(GL_TEXTURE_2D);

                //Bind the Texture to the Object
                material.textureID = textureID;

                //Free the Image Data
                stbi_image_free(image);
            }
        }

        /**
         * @brief Read - Reads the Files and Creates a List of Object Data
         *
         * @param Folder : The Folder of the Files
         * @return : Object Data List
         */
        static std::vector<std::pair<std::vector<Vertex>, Material>> Read(const std::string& obj_model_folderpath) {

            //The List of Objects that will be Populated with the Objects Vertices and Material
            std::vector<std::pair<std::vector<Vertex>, Material>> objDataList;

            for (size_t i = 1; i < 16; ++i) {
                std::string obj_model_filepath = obj_model_folderpath + "Ball" + std::to_string(i) + ".obj";
                std::string mtlFileName;
                std::ifstream objFile(obj_model_filepath);

                if (!objFile) {
                    throw std::runtime_error("Failed to open OBJ file: " + obj_model_filepath);
                }

                Material material;
                std::vector<Vertex> vertices;
                std::vector<glm::vec3> positions;
                std::vector<glm::vec2> texcoords;
                std::vector<glm::vec3> normals;

                std::string line;
                while (std::getline(objFile, line)) {
                    std::istringstream iss(line);
                    std::string prefix;
                    iss >> prefix;

                    if (prefix == "v") {
                        //Vertex Position
                        glm::vec3 position;
                        iss >> position.x >> position.y >> position.z;
                        positions.push_back(position);
                    }
                    else if (prefix == "vt") {
                        //Vertex Texture Coordinates
                        glm::vec2 texcoord;
                        iss >> texcoord.x >> texcoord.y;
                        texcoords.push_back(texcoord);
                    }
                    else if (prefix == "vn") {
                        //Vetex Normal
                        glm::vec3 normal;
                        iss >> normal.x >> normal.y >> normal.z;
                        normals.push_back(normal);
                    }
                    else if (prefix == "mtllib") {
                        //Material File Name
                        iss >> mtlFileName;
                    }
                    else if (prefix == "f") {
                        //Faces
                        std::string faceToken;
                        while (iss >> faceToken) {
                            std::replace(faceToken.begin(), faceToken.end(), '/', ' ');
                            std::istringstream faceIss(faceToken);
                            int vertexIndex, texcoordIndex, normalIndex;
                            faceIss >> vertexIndex >> texcoordIndex >> normalIndex;

                            //Creates a Vertex with the Corresponding Position, Texture Coordinates and Normal
                            Vertex vertex;
                            vertex.position = positions[vertexIndex - 1];
                            vertex.texcoord = texcoords[texcoordIndex - 1];
                            vertex.normal = normals[normalIndex - 1];
                            vertices.emplace_back(vertex);
                        }
                    }
                }

                //Load Materials from MTL File
                std::string mtlPath = obj_model_folderpath.substr(0, obj_model_folderpath.find_last_of('/')) + "/" + mtlFileName;
                std::ifstream mtlFile(mtlPath);
                if (mtlFile) {
                    std::string mtlLine;
                    while (std::getline(mtlFile, mtlLine)) {
                        std::istringstream mtlIss(mtlLine);
                        std::string mtlPrefix;
                        mtlIss >> mtlPrefix;

                        if (mtlPrefix == "newmtl") {
                            //Material Name
                            mtlIss >> material.name;
                        }
                        else if (mtlPrefix == "Ka") {
                            //Ambient Reflection Coefficient
                            mtlIss >> material.ambient.r >> material.ambient.g >> material.ambient.b;
                        }
                        else if (mtlPrefix == "Kd") {
                            //Difuse Reflection coefficient
                            mtlIss >> material.diffuse.r >> material.diffuse.g >> material.diffuse.b;
                        }
                        else if (mtlPrefix == "Ks") {
                            //Specular Reflection coefficient
                            mtlIss >> material.specular.r >> material.specular.g >> material.specular.b;
                        }
                        else if (mtlPrefix == "Ns") {
                            //Specular exponent
                            mtlIss >> material.shininess;
                        }
                        else if (mtlPrefix == "map_Kd") {
                            //Material Texture
                            mtlIss >> material.textureFile;
                            material.textureFile = obj_model_folderpath.substr(0, obj_model_folderpath.find_last_of('/')) + "/" + material.textureFile;
                        }
                    }
                }
                else {

                    std::cerr << "Failed to open MTL file: " << mtlPath << std::endl;
                }

                //Creates a Pair of Vertices and Material, and Add it to the Object Data
                std::pair<std::vector<Vertex>, Material> objectData = std::make_pair(vertices, material);
                objDataList.push_back(objectData);
            }

            //Load the Textures for the Objects Data
            LoadTextures(&objDataList);

            return objDataList;
        }

        /**
         * @brief CreateVertexBuffer - Creates a Vertex Buffer Obejct (VBO) and Filles it with the Given Vertex Data
         *
         * @param Vertices : The Vector of Vertices to be Stored in the VBO
         * @return : The Generated VBO
         */
        static GLuint CreateVertexBuffer(const std::vector<Vertex>& vertices) {
            GLuint vbo;

            //Generates the Buffer Object for the VBO
            glGenBuffers(1, &vbo);

            //Bind the VBO as an Array Buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            //Fills the VBO with the Vertices Data
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            //Unbinds the VBO
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //Retruns the Generated VBO ID
            return vbo;
        }

        /**
         * @brief BindVertexBuffer - Binds a Vertex Buffer Obejct (VBO) and Sets Up the Vertices Attribute Pointers
         *
         * @param VBO : The ID of the VBO to Bind
         * @return : Void
         */
        static void BindVertexArray(const GLuint vbo) {

            //Binds the VBO as an Array Buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            //Enables and Sets Up the Vertex Pointers, the Normal Pointers and the Texture Coordinate Pointers
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, position)));
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, normal)));
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, texcoord)));
        }

        /**
         * @brief Send - Creates a Vertex Buffer Obejct (VBO) and Binds it, then sets up Vertex attribute pointers
         *
         * @param Vertices : The Vector of Vertices to be Stored in the VBO
         * @return : The Generated VBO
         */
        static std::vector<GLuint> Send(std::vector<std::pair<std::vector<Vertex>, Material>>& ObjectDataList)
        {
            std::vector<GLuint> vboIDs;

            //Create and Bind the Vertex Buffer Object for each Object in ObjectDataList
            for (auto& ObjectData : ObjectDataList) {
                GLuint vbo = CreateVertexBuffer(ObjectData.first);
                BindVertexArray(vbo);
                vboIDs.push_back(vbo);
            }

            //Retruns a List of the Generated VBO ID
            return vboIDs;
        }

        /**
         * @brief ReadShaderFile - Reads the Contents of a Shader File and returns it as a string
         *
         * @param FileName : The Name of the Shader File
         * @return : The Contents of the Shader File
         */
        static std::string readShaderFile(const std::string& filename) {

            //Opens the File
            std::ifstream file(filename);
            if (!file) {
                //If the File Failed to Open, Display an Error
                std::cerr << "Failed to open file: " << filename << std::endl;
                return "";
            }

            //Creates a String Stream to Hold the File Contents
            std::stringstream buffer;

            //Reads the File Contents into the String Stream
            buffer << file.rdbuf();

            //Convertes the Sting Stream to a String and Returns it
            return buffer.str();
        }

        /**
         * @brief CreateShaderProgram - Creates a Shader Program
         *
         * @param vertexShaderCode : The Vertex Shader Code to be Compiled
         * @param fragmentShaderCode : The Fragment Shader Code to be Compiled
         * @return : The ID of the Created Shader Program
         */
        static GLuint createShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) {

            //Creates the Vertex Shader
            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char* vertexShaderCodePtr = vertexShaderCode.c_str();

            //Replaces the Source Code of the Shader Object and Compiles it
            glShaderSource(vertexShader, 1, &vertexShaderCodePtr, nullptr);
            glCompileShader(vertexShader);

            //Checks Vertex Shader Compilation Status
            GLint vertexShaderStatus;
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);

            //Outputs error log if the status is GL_FALSE
            if (vertexShaderStatus != GL_TRUE) {
                GLint infoLogLength;
                glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
                GLchar* infoLog = new GLchar[infoLogLength];
                glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, infoLog);
                std::cerr << "Vertex shader compilation error: " << infoLog << std::endl;
                delete[] infoLog;
                glDeleteShader(vertexShader);
                return 0;
            }

            //Creates the FRagment Shader
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* fragmentShaderCodePtr = fragmentShaderCode.c_str();

            //Replaces the Source Code of the Fragment Object and Compiles it
            glShaderSource(fragmentShader, 1, &fragmentShaderCodePtr, nullptr);
            glCompileShader(fragmentShader);

            //Check fragment shader compilation status
            GLint fragmentShaderStatus;
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderStatus);

            //Outputs error log if the status is GL_FALSE
            if (fragmentShaderStatus != GL_TRUE) {
                GLint infoLogLength;
                glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
                GLchar* infoLog = new GLchar[infoLogLength];
                glGetShaderInfoLog(fragmentShader, infoLogLength, nullptr, infoLog);
                std::cerr << "Fragment shader compilation error: " << infoLog << std::endl;
                delete[] infoLog;
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                return 0;
            }

            //Creates the Shader Program
            GLuint shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);

            //Creates an Executable That can be Used by the GPU
            glLinkProgram(shaderProgram);

            //Checks Vertex Shader Compilation Status
            GLint shaderProgramStatus;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramStatus);

            //Outputs error log if the status is GL_FALSE
            if (shaderProgramStatus != GL_TRUE) {
                GLint infoLogLength;
                glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
                GLchar* infoLog = new GLchar[infoLogLength];
                glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, infoLog);
                std::cerr << "Shader program linking error: " << infoLog << std::endl;
                delete[] infoLog;
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                glDeleteProgram(shaderProgram);
                return 0;
            }

            //Cleans Up Shaders from Local Memory
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            //Returns the Created Shader Program's ID
            return shaderProgram;
        }

        /**
         * @brief Draw - Draws an Object
         *
         * @param Position : The Position of the Object
         * @param Orientation : The Orie ntationof the Object
         * @param ObjectData : The List Containing the Object Data (Vertex and Material)
         * @param VBO : The ID of the Vertex Buffer Object (VBO)
         * @param ShaderProgram : The Id of the Shader Program
         * @return : Void
         */
        static void Draw(const glm::vec3& position, const glm::vec3& orientation, const std::pair<std::vector<Vertex>, Material>& objData, GLuint vbo/*, GLuint shaderProgram*/) {

            const std::vector<Vertex>& vertices = objData.first;
            const Material& material = objData.second;

            //Bind the Existing VBO to Render
            BindVertexArray(vbo);

            //Set the Position and Orientation of the Object
            glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glRotatef(orientation.x, 1.0f, 0.0f, 0.0f);
            glRotatef(orientation.y, 0.0f, 1.0f, 0.0f);
            glRotatef(orientation.z, 0.0f, 0.0f, 1.0f);

            //Bind the Texture to the Object
            glBindTexture(GL_TEXTURE_2D, material.textureID);

            // Set the material properties

            /*glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"),  material.textureID);
            glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(material.ambient));
            glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
            glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(material.specular));
            glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), material.shininess);*/

            //Material properties TODO: Replace with Shader Program
            glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(glm::vec4(material.ambient, 1.0f)));
            glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(glm::vec4(material.diffuse, 1.0f)));
            glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(glm::vec4(material.specular, 1.0f)));
            glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);


            //Render the Object using the Vertex Array Object (VBO)
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());

            //Cleanup
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        }

    };
}