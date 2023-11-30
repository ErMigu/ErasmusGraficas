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

class matrixRoutinesAndOBJ {
    public:

    /**Direct translation function**/
    static glm::mat4 translate(float dx, float dy, float dz) {
        glm::mat4 M{
                {1, 0, 0, dx},
                {0, 1, 0, dy},
                {0, 0, 1, dz},
                {0, 0, 0, 1}
        };

        return M;
    }

    /**Direct scaling function**/
    static glm::mat4 scale(float sx, float sy, float sz) {
        glm::mat4 M{
                {sx, 0, 0, 0},
                {0, sy, 0, 0},
                {0, 0, sz, 0},
                {0, 0, 0, 1}
        };

        return M;
    }

    /**Direct rotation**/
    static glm::mat4 rotate(const std::vector<glm::vec4>& vectors, float degreex, float degreey, float degreez, glm::mat4 matModel) {
        //getting the center of the object
        glm::vec4 center(0.0f, 0.0f, 0.0f,1.0f);
        center = calculateCenter(vectors,matModel);

        //translate to 0,0,0
        glm::mat4 M1 = translate(-center.x, -center.y, -center.z);

        //rotate
        glm::mat4 M2;
        if (degreex != 0.0f) { // rotatex
            float rad = degreex * M_PI / 180.0f;
            M2 = {
                    {1, 0, 0, 0},
                    {0, std::cos(rad), -std::sin(rad), 0},
                    {0, std::sin(rad), std::cos(rad), 0},
                    {0, 0, 0, 1}
            };
        } else if (degreey != 0.0f) { // rotatey
            float rad = degreey * M_PI / 180.0f;
            M2 = {
                    {std::cos(rad), 0, std::sin(rad), 0},
                    {0, 1, 0, 0},
                    {-std::sin(rad), 0, std::cos(rad), 0},
                    {0, 0, 0, 1}
            };
        } else if (degreez != 0.0f) { // rotatez
            float rad = degreez * M_PI / 180.0f;
            M2 = {
                    {std::cos(rad), -std::sin(rad), 0, 0},
                    {std::sin(rad), std::cos(rad), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}
            };
        }

        //translate to the original place
        glm::mat4 M3 = translate(center.x, center.y, center.z);
        return M3*M2*M1;
    }


//----------------------------------------


    /**Reads the OBJfile**/
    static void readOBJ(const std::string& name, std::vector<glm::vec4>& vertices, std::vector<unsigned int>& indices) {
        vertices.clear();
        indices.clear();

        std::ifstream objFile("./Objs/" + name);
        if (!objFile.is_open()) {
            throw std::runtime_error("Unable to open OBJ file!");
        }
        std::string line;
        while (std::getline(objFile, line)) { //for each line
            std::istringstream lineStream(line);
            std::string prefix;
            lineStream >> prefix;
            if (prefix == "v") { //read if the first is a v
                glm::vec4 vertex;
                lineStream >> vertex.x >> vertex.y >> vertex.z; //save vertex
                vertices.push_back(glm::vec4(vertex.x,vertex.y,vertex.z,1.0f));
            } else if (prefix == "f") { //read if the first is an 'f'
                char nextChar;
                for (int i = 0; i < 3; ++i) {
                    unsigned int index=0;
                    while (!isspace(lineStream.peek())) { //deleting everything is not a space
                        lineStream.get();
                    }
                    while (isspace(lineStream.peek())) { //we got the space, so we delete it
                        lineStream.get();
                    }
                    while (isdigit(lineStream.peek())) { //we keep every consecutive digit, case: 33,231,...
                        lineStream >> nextChar;
                        index = index * 10 + (nextChar - '0'); //build the number
                    }
                    indices.push_back(index - 1); // obj indexes start at 1 so we subtract 1
                }
            }

        }
        objFile.close();
        normalizeObject(vertices,true);
        for(int i=0; i<vertices.size(); i++){
            std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
        }std::cout << std::endl;

        for(int i=0; i<indices.size(); i++){
            std::cout << indices[i];
        }
    }

    /**Make it fits in a 1x1x1 cube and center y the 0,0,0**/
    static glm::mat4 normalizeObject(std::vector<glm::vec4>& vertices, bool changeVertices) {
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

        //normalize
        glm::mat4 aux= matrixRoutinesAndOBJ::scale(scale,scale,scale);
        if(changeVertices){ //to apply to the vertex directly only the first time, and move it to 0,0,0
            for(unsigned int i=0; i<vertices.size(); i++){
                vertices[i].x=vertices[i].x*scale;
                vertices[i].y=vertices[i].y*scale;
                vertices[i].z=vertices[i].z*scale;
            }
        }
        return aux;
    }

    /**It returns the center point of the object**/
    static glm::vec4 calculateCenter(const std::vector<glm::vec4>& vectors, glm::mat4 matModel) {
        //var
        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float minZ = std::numeric_limits<float>::max();
        float maxX = -std::numeric_limits<float>::max();
        float maxY = -std::numeric_limits<float>::max();
        float maxZ = -std::numeric_limits<float>::max();

        //apply mat and see if is max min or nothing
        for (const glm::vec4& vec : vectors) {
            glm::vec4 aux= apply3Matrix(vec,matModel);
            minX = std::min(minX, aux.x);
            minY = std::min(minY, aux.y);
            minZ = std::min(minZ, aux.z);
            maxX = std::max(maxX, aux.x);
            maxY = std::max(maxY, aux.y);
            maxZ = std::max(maxZ, aux.z);
        }

        //calculate center
        float centerX = (minX + maxX) / 2.0f;
        float centerY = (minY + maxY) / 2.0f;
        float centerZ = (minZ + maxZ) / 2.0f;

        return glm::vec4(centerX, centerY, centerZ,1.0f);
    }

    /**get real position that I see on the screen**/
    static glm::vec4 apply3Matrix(glm::vec4 point, glm::mat4 mat) {
        glm::vec4 tempPoint = mat * glm::vec4(point);
        return glm::vec4(tempPoint.x, tempPoint.y, tempPoint.z,1.0f);
    }
};


//----------------------------------------


