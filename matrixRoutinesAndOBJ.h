//
// Created by migue on 02/11/2023.
//
#include <vector>
#include <limits>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dstudio.h"
#include "geometryrender.h"
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

class matrixRoutinesAndOBJ { public:
//----------------------------------------


    /**Direct translation function**/
    static glm::mat4 translate(float x, float y, float z) {
        return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    }

    /**Direct scaling function**/
    static glm::mat4 scale(float x, float y, float z, glm::mat4 matModel) {
        return glm::scale(matModel, glm::vec3(x, y, z));
    }

    /**Direct rotation**/
    static glm::mat4 rotate(float degreex, float degreey, float degreez, glm::mat4 matModel) {
        if(degreex != 0)
            return glm::rotate(matModel, glm::radians(degreex), glm::vec3(1, 0, 0));
        else if(degreey != 0)
            return glm::rotate(matModel, glm::radians(degreey), glm::vec3(0, 1, 0));
        else if(degreez != 0)
            return glm::rotate(matModel, glm::radians(degreez), glm::vec3(0, 0, 1));
    }


//----------------------------------------


    /**Reads the OBJfile**/
    static void readOBJ(const std::string& name, std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& normals, std::vector<unsigned int>& indices,  std::vector<unsigned int>& indicesN, std::vector<glm::vec2>& textcoords) {
        std::vector<glm::vec4> normalsAux;
        vertices.clear();
        normals.clear();
        indices.clear();
        indicesN.clear();

        std::ifstream objFile("./Objs/" + name);
        if (!objFile.is_open()) {
            throw std::runtime_error("Unable to open OBJ file!");
        }
        std::string line;
        while (std::getline(objFile, line)) { //for each line
            std::istringstream lineStream(line);
            std::string prefix;
            lineStream >> prefix;
            if (prefix == "v") {
                glm::vec4 vertex;
                lineStream >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(glm::vec4(vertex.x,vertex.y,vertex.z,1.0f));
            } else if (prefix == "vn"){
                glm::vec4 normal;
                lineStream >> normal.x >> normal.y >> normal.z;
                normals.push_back(glm::vec4(normal.x,normal.y,normal.z,0.0f));
            } else if (prefix == "f") { //MAIN LOOPS RN
                char nextChar;
                std::vector<unsigned int> aux;

                while(lineStream.peek()!=EOF) { //NUMBERS FROM LINE TO ARRAY
                    unsigned int index=0;
                    while (!isdigit(lineStream.peek())) { //we delete everything is not a digit
                        lineStream.get();
                    }
                    while (isdigit(lineStream.peek())) { //we keep every consecutive digit, case: 33,231,...
                        lineStream >> nextChar;
                        index = index * 10 + (nextChar - '0'); //build the number
                    }
                    aux.push_back(index-1);
                }

                if (normals.empty()==false) { //CASE WE NEED TO READ NORMALS INDEX
                    if (aux.size() == 6) { //CASE X//X X//X X//X
                        indices.push_back(aux[0]); indices.push_back(aux[2]); indices.push_back(aux[4]);
                        indicesN.push_back(aux[1]); indicesN.push_back(aux[3]); indicesN.push_back(aux[5]);
                    } else if (aux.size() == 9) { //CASE X/X/X X/X/X X/X/X
                        indices.push_back(aux[0]); indices.push_back(aux[3]); indices.push_back(aux[6]);
                        indicesN.push_back(aux[2]); indicesN.push_back(aux[5]); indicesN.push_back(aux[8]);
                    } else if (aux.size() == 8) { //CASE X//X X//X X//X X//X
                        indices.push_back(aux[0]); indices.push_back(aux[2]); indices.push_back(aux[4]);
                        indices.push_back(aux[4]); indices.push_back(aux[6]); indices.push_back(aux[0]);
                        indicesN.push_back(aux[1]); indicesN.push_back(aux[3]); indicesN.push_back(aux[5]);
                        indicesN.push_back(aux[5]); indicesN.push_back(aux[7]); indicesN.push_back(aux[1]);
                    } else if (aux.size() == 12) { //CASE X/X/X X/X/X X/X/X X/X/X
                        indices.push_back(aux[0]); indices.push_back(aux[3]); indices.push_back(aux[6]);
                        indices.push_back(aux[6]); indices.push_back(aux[9]); indices.push_back(aux[0]);
                        indicesN.push_back(aux[2]); indicesN.push_back(aux[5]); indicesN.push_back(aux[8]);
                        indicesN.push_back(aux[8]); indicesN.push_back(aux[11]); indicesN.push_back(aux[2]);
                    }

                } else { //CASE WE NEED TO CALCULATE NORMALS
                    if (aux.size() == 3) { //CASE X X X
                        indices.push_back(aux[0]); indices.push_back(aux[1]); indices.push_back(aux[2]);
                    } else if (aux.size() == 4) { //CASE X X X X
                        indices.push_back(aux[0]); indices.push_back(aux[1]); indices.push_back(aux[2]);
                        indices.push_back(aux[2]); indices.push_back(aux[3]); indices.push_back(aux[0]);
                    } else if (aux.size() == 6) { //CASE X//X X//X X//X
                        indices.push_back(aux[0]); indices.push_back(aux[2]); indices.push_back(aux[4]);
                    } else if (aux.size() == 9) { //CASE X/X/X X/X/X X/X/X
                        indices.push_back(aux[0]); indices.push_back(aux[3]); indices.push_back(aux[6]);
                    } else if (aux.size() == 8) { //CASE X//X X//X X//X X//X
                        indices.push_back(aux[0]); indices.push_back(aux[2]); indices.push_back(aux[4]);
                        indices.push_back(aux[4]); indices.push_back(aux[6]); indices.push_back(aux[0]);
                    } else if (aux.size() == 12) { //CASE X/X/X X/X/X X/X/X X/X/X
                        indices.push_back(aux[0]); indices.push_back(aux[3]); indices.push_back(aux[6]);
                        indices.push_back(aux[6]); indices.push_back(aux[9]); indices.push_back(aux[0]);
                    }
                }
            }
        }
        objFile.close();

        if (normalsAux.empty()) {
            normals.resize(vertices.size(), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            indicesN.resize(indices.size());

            for (size_t i = 0; i < indices.size(); i += 3) {
                // Extrae los vértices de cada triángulo
                glm::vec3 v0 = glm::vec3(vertices[indices[i]]);
                glm::vec3 v1 = glm::vec3(vertices[indices[i + 1]]);
                glm::vec3 v2 = glm::vec3(vertices[indices[i + 2]]);

                // Calcula las aristas del triángulo
                glm::vec3 edge1 = v1 - v0;
                glm::vec3 edge2 = v2 - v0;

                // Calcula la normal de la cara (no normalizada para ponderar por área de la cara)
                glm::vec3 faceNormal = glm::cross(edge1, edge2);

                // Agrega la normal de la cara a las normales de los vértices
                for (int j = 0; j < 3; ++j) {
                    int vertexIndex = indices[i + j];
                    normals[vertexIndex] += glm::vec4(faceNormal, 0.0f);
                    indicesN[i + j] = vertexIndex; // Aquí asumimos que el índice de la normal es el mismo que el índice del vértice
                }
            }

            // Normaliza las normales de los vértices
            for (auto& normal : normals) {
                normal = glm::vec4(glm::normalize(glm::vec3(normal)), 0.0f);
            }
        }
        normalizeObject(vertices,normals,true);

        for (const auto& vertex : vertices) {
            float u = atan2(vertex.z, vertex.x) / (2 * M_PI) + 0.5f;
            float v = asin(vertex.y) / M_PI + 0.5f;
            textcoords.push_back(glm::vec2(u, v));
        }

        /*std::cout << "Imprimiendo coordenadas de los vertices:" << std::endl;
        for(int i = 0; i < vertices.size(); i++){
            std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Imprimiendo coordenadas de las normales:" << std::endl;
        for(int i = 0; i < normals.size(); i++){
            std::cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Imprimiendo indices de vertices:" << std::endl;
        for(int i = 0; i < indices.size(); i++){
            std::cout << indices[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Imprimiendo indices de normales:" << std::endl;
        for(int i = 0; i < indicesN.size(); i++){
            std::cout << indicesN[i] << " ";
        }
        std::cout << std::endl;*/
    }

    /**Make it fits in a 1x1x1 cube and center y the 0,0,0**/
    static glm::mat4 normalizeObject(std::vector<glm::vec4>& vertices, std::vector<glm::vec4>& normals, bool changeVertices) {
        //max, min values for x,y,z
        glm::vec4 minVertex = vertices[0];
        glm::vec4 maxVertex = vertices[0];

        for (const auto& vertex : vertices) {
            if (vertex.x < minVertex.x) minVertex.x=vertex.x;
            if (vertex.y < minVertex.y) minVertex.y=vertex.y;
            if (vertex.z < minVertex.z) minVertex.z=vertex.z;

            if (vertex.x > maxVertex.x) maxVertex.x=vertex.x;
            if (vertex.y > maxVertex.y) maxVertex.y=vertex.y;
            if (vertex.z > maxVertex.z) maxVertex.z=vertex.z;
        }

        //calculate scale
        float maxXRange = maxVertex.x - minVertex.x;
        float maxYRange = maxVertex.y - minVertex.y;
        float maxZRange = maxVertex.z - minVertex.z;

        float maxRange = std::max(maxXRange, std::max(maxYRange, maxZRange));
        float scale = 1.0f / maxRange;

        //Calculate center
        glm::vec4 center=calculateCenter(vertices);

        //normalize
        glm::mat4 aux= matrixRoutinesAndOBJ::scale(scale,scale,scale,glm::mat4(1.0f));
        if(changeVertices){ //to apply to the vertex directly only the first time, and move it to 0,0,0
            for(unsigned int i=0; i<vertices.size(); i++){
                vertices[i].x=vertices[i].x-center.x;
                vertices[i].x=vertices[i].x*scale;
                vertices[i].y=vertices[i].y-center.y;
                vertices[i].y=vertices[i].y*scale;
                vertices[i].z=vertices[i].z-center.z;
                vertices[i].z=vertices[i].z*scale;
            }
            for(unsigned int i=0; i<normals.size(); i++){
                normals[i] = glm::normalize(normals[i]);
            }
        }
        return aux;
    }

    /**It returns the center point of the object**/
    static glm::vec4 calculateCenter(const std::vector<glm::vec4>& vectors) {
        //var
        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float minZ = std::numeric_limits<float>::max();
        float maxX = -std::numeric_limits<float>::max();
        float maxY = -std::numeric_limits<float>::max();
        float maxZ = -std::numeric_limits<float>::max();

        //apply mat and see if is max min or nothing
        for (const glm::vec4& vec : vectors) {
            minX = std::min(minX, vec.x);
            minY = std::min(minY, vec.y);
            minZ = std::min(minZ, vec.z);
            maxX = std::max(maxX, vec.x);
            maxY = std::max(maxY, vec.y);
            maxZ = std::max(maxZ, vec.z);
        }

        //calculate center
        float centerX = (minX + maxX) / 2.0f;
        float centerY = (minY + maxY) / 2.0f;
        float centerZ = (minZ + maxZ) / 2.0f;

        return glm::vec4(centerX, centerY, centerZ,1.0f);
    }


//----------------------------------------
};