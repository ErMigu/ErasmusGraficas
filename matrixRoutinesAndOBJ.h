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

class matrixRoutinesAndOBJ {
    public:
//-----------------------------------------

    /**Matrix4x4 X Matrix4x4**/
    static std::vector<std::vector<float>> mulMatrix4x4(const std::vector<std::vector<float>> &m1, const std::vector<std::vector<float>> &m2) {
        std::vector<std::vector<float>> result(4, std::vector<float>(4, 0));
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    result[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }
        return result;
    }


    /**Matrix4x4 X Matrix4x1**/
    static std::vector<Vec3> mulMatrixVectorR(const std::vector<std::vector<float>>& matrix, const std::vector<Vec3>& vectors) {
        std::vector<Vec3> result;
        for (const Vec3& vec : vectors) {
            std::vector<float> inputVec{vec.x(), vec.y(), vec.z(), 1.0f};
            std::vector<float> resultVec(inputVec.size(), 0.0f);

            for (size_t i = 0; i < matrix.size(); ++i) {
                for (size_t j = 0; j < inputVec.size(); ++j) {
                    resultVec[i] += matrix[i][j] * inputVec[j];
                }
            }
            result.push_back(Vec3(resultVec[0], resultVec[1], resultVec[2]));
        }
        return result;
    }

    /**Adding w=1 component (to translate)**/
    static std::vector<Vec3> mulMatrixVectorT(const std::vector<std::vector<float>>& matrix, const std::vector<Vec3>& vectors) {
        std::vector<Vec3> result;
        for (const Vec3& vec : vectors) {
            //convert the 3D vector to a 4D vector (adding component w = 1)
            std::vector<float> inputVec{vec.x(), vec.y(), vec.z(), 1.0f};
            std::vector<float> resultVec(inputVec.size(), 0.0f);

            for (size_t i = 0; i < matrix.size(); ++i) {
                for (size_t j = 0; j < inputVec.size(); ++j) {
                    resultVec[i] += matrix[i][j] * inputVec[j];
                }
            }

            //delete the extra component
            result.push_back(Vec3(resultVec[0] / resultVec[3], resultVec[1] / resultVec[3], resultVec[2] / resultVec[3]));
        }
        return result;
    }


//-----------------------------------------

    /**Direct translation function**/
    static std::vector<std::vector<float>> translate(float dx, float dy, float dz) {
        std::vector<std::vector<float>> M{
                {1, 0, 0, dx},
                {0, 1, 0, dy},
                {0, 0, 1, dz},
                {0, 0, 0, 1}
        };

        return M;
    }

    /**Direct scaling function**/
    static std::vector<std::vector<float>> scale(float sx, float sy, float sz) {
        std::vector<std::vector<float>> M{
                {sx, 0, 0, 0},
                {0, sy, 0, 0},
                {0, 0, sz, 0},
                {0, 0, 0, 1}
        };

        return M;
    }

    /**ROTATION NEED DEGREE -> RAD**/
    /**Direct rotation around x-axis**/
    static std::vector<std::vector<float>> rotatex(const std::vector<Vec3>& vectors, float degree, Mat4x4 matModel) {
        //getting the center of the object
        Vec3 center(0.0f, 0.0f, 0.0f);
        center= calculateCenter(vectors,matModel);

        //translate to 0,0,0
        std::vector<std::vector<float>> M1 = translate(-center.x(), -center.y(), -center.z());

        //rotate
        float rad = degree * M_PI / 180.0f;
        std::vector<std::vector<float>> M2{
                {1, 0, 0, 0},
                {0, std::cos(rad), -std::sin(rad), 0},
                {0, std::sin(rad), std::cos(rad), 0},
                {0, 0, 0, 1}
        };

        //translate to the original place
        std::vector<std::vector<float>> M3 = translate(center.x(), center.y(), center.z());
        std::vector<std::vector<float>> aux = mulMatrix4x4(M3,mulMatrix4x4(M2,M1));
        return aux;
    }

    /**Direct rotation around y-axis**/
    static std::vector<std::vector<float>> rotatey(const std::vector<Vec3>& vectors, float degree, Mat4x4 matModel) {
        //getting the center of the object
        Vec3 center(0.0f, 0.0f, 0.0f);
        center= calculateCenter(vectors,matModel);

        //translate to 0,0,0
        std::vector<Vec3> translatedVectors;
        std::vector<std::vector<float>> M1 = translate(-center.x(), -center.y(), -center.z());

        //rotate
        float rad = degree * M_PI / 180.0f;
        std::vector<std::vector<float>> M2{
                {std::cos(rad), 0, -std::sin(rad), 0},
                {0, 1, 0, 0},
                {std::sin(rad), 0, std::cos(rad), 0},
                {0, 0, 0, 1}
        };

        //translate to the original place
        std::vector<std::vector<float>> M3 = translate(center.x(), center.y(), center.z());
        return mulMatrix4x4(M3,mulMatrix4x4(M2,M1));
    }

    /**Direct rotation around y-axis**/
    static std::vector<std::vector<float>> rotatez(const std::vector<Vec3>& vectors, float degree, Mat4x4 matModel) {
        //getting the center of the object
        Vec3 center(0.0f, 0.0f, 0.0f);
        center= calculateCenter(vectors,matModel);

        //translate to 0,0,0
        std::vector<Vec3> translatedVectors;
        std::vector<std::vector<float>> M1 = translate(-center.x(), -center.y(), -center.z());

        //rotate
        float rad = degree * M_PI / 180.0f;
        std::vector<std::vector<float>> M2{
                {std::cos(rad), -std::sin(rad), 0, 0},
                {std::sin(rad), std::cos(rad), 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}
        };

        //translate to the original place
        std::vector<std::vector<float>> M3 = translate(center.x(), center.y(), center.z());
        return mulMatrix4x4(M3,mulMatrix4x4(M2,M1));
    }


//-----------------------------------------

    static void readOBJ(const std::string& name, std::vector<Vec3>& vertices, std::vector<unsigned int>& indices) {
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
                Vec3 vertex;
                lineStream >> vertex.values[0] >> vertex.values[1] >> vertex.values[2]; //save vertex
                vertices.push_back(vertex);
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
    }

    ///TODO preguntar el tema de la normalizacion al profe, es decir si tiene que estar realmente normalizado en coordenadas o TOOOOODO pasa por el filtro y punto.
    //make it fits in a 1x1x1 cube and center y the 0,0,0
    static std::vector<std::vector<float>> normalizeObject(std::vector<Vec3>& vertices) {
        //max, min values for x,y,z
        Vec3 minVertex = vertices[0];
        Vec3 maxVertex = vertices[0];

        for (const auto& vertex : vertices) {
            if (vertex.x() < minVertex.x()) minVertex.x(vertex.x());
            if (vertex.y() < minVertex.y()) minVertex.y(vertex.y());
            if (vertex.z() < minVertex.z()) minVertex.z(vertex.z());

            if (vertex.x() > maxVertex.x()) maxVertex.x(vertex.x());
            if (vertex.y() > maxVertex.y()) maxVertex.y(vertex.y());
            if (vertex.z() > maxVertex.z()) maxVertex.z(vertex.z());
        }

        //calculate scale
        float maxXRange = maxVertex.x() - minVertex.x();
        float maxYRange = maxVertex.y() - minVertex.y();
        float maxZRange = maxVertex.z() - minVertex.z();

        float maxRange = std::max(maxXRange, std::max(maxYRange, maxZRange));
        float scale = 1.0f / maxRange;

        //normalize
        std::vector<std::vector<float>> aux= matrixRoutinesAndOBJ::scale(scale,scale,scale);
        for(unsigned int i=0; i<vertices.size(); i++){
            vertices[i].x(vertices[i].x()*scale);
            vertices[i].y(vertices[i].y()*scale);
            vertices[i].z(vertices[i].z()*scale);
        }

        return aux;
    }

    static Vec3 calculateCenter(const std::vector<Vec3>& vectors, Mat4x4 matModel) {
        //var
        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float minZ = std::numeric_limits<float>::max();
        float maxX = -std::numeric_limits<float>::max();
        float maxY = -std::numeric_limits<float>::max();
        float maxZ = -std::numeric_limits<float>::max();

        //apply mat and see if is max min or nothing
        for (const Vec3& vec : vectors) {
            float x = matModel[0] * vec.x() + matModel[1] * vec.y() + matModel[2] * vec.z() + matModel[3];
            float y = matModel[4] * vec.x() + matModel[5] * vec.y() + matModel[6] * vec.z() + matModel[7];
            float z = matModel[8] * vec.x() + matModel[9] * vec.y() + matModel[10] * vec.z() + matModel[11];
            minX = std::min(minX, x);
            minY = std::min(minY, y);
            minZ = std::min(minZ, z);
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);
            maxZ = std::max(maxZ, z);
        }

        //calculate center
        float centerX = (minX + maxX) / 2.0f;
        float centerY = (minY + maxY) / 2.0f;
        float centerZ = (minZ + maxZ) / 2.0f;

        return Vec3(centerX, centerY, centerZ);
    }

};




