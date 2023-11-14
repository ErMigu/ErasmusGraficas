//
// Created by migue on 02/11/2023.
//
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "3dstudio.h"

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

    static std::vector<Vec3> mulMatrixVectorT(const std::vector<std::vector<float>>& matrix, const std::vector<Vec3>& vectors) {
        std::vector<Vec3> result;
        for (const Vec3& vec : vectors) {
            // Convertir el vector 3D a un vector 4D (agregando una componente w = 1)
            std::vector<float> inputVec{vec.x(), vec.y(), vec.z(), 1.0f};
            std::vector<float> resultVec(inputVec.size(), 0.0f);

            for (size_t i = 0; i < matrix.size(); ++i) {
                for (size_t j = 0; j < inputVec.size(); ++j) {
                    resultVec[i] += matrix[i][j] * inputVec[j];
                }
            }

            // Eliminar la cuarta componente (coordenada homogénea) después de la multiplicación
            result.push_back(Vec3(resultVec[0] / resultVec[3], resultVec[1] / resultVec[3], resultVec[2] / resultVec[3]));
        }
        return result;
    }


//-----------------------------------------

    /**Direct translation function**/
        static std::vector<Vec3> translate(const std::vector<Vec3>& vectors, float dx, float dy, float dz) {
            std::vector<std::vector<float>> M{
                    {1, 0, 0, dx},
                    {0, 1, 0, dy},
                    {0, 0, 1, dz},
                    {0, 0, 0, 1}
            };

            return mulMatrixVectorT(M, vectors);
        }

    /**Direct scaling function**/
        static std::vector<Vec3> scale(const std::vector<Vec3>& vectors, float sx, float sy, float sz) {
            std::vector<std::vector<float>> M{
                    {sx, 0, 0, 0},
                    {0, sy, 0, 0},
                    {0, 0, sz, 0},
                    {0, 0, 0, 1}
            };

            return mulMatrixVectorR(M, vectors);
        }

    /**Direct rotation around x-axis**/
        static std::vector<Vec3> rotatex(const std::vector<Vec3>& vectors, float degree) {
            float rad = degree * M_PI / 180.0f;

            std::vector<std::vector<float>> M{
                    {1, 0, 0, 0},
                    {0, std::cos(rad), -std::sin(rad), 0},
                    {0, std::sin(rad), std::cos(rad), 0},
                    {0, 0, 0, 1}
            };

            return mulMatrixVectorR(M, vectors);
        }

    /**Direct rotation around y-axis**/
        static std::vector<Vec3> rotatey(const std::vector<Vec3>& vectors, float degree) {
            float rad = degree * M_PI / 180.0f;

            std::vector<std::vector<float>> M{
                    {std::cos(rad), 0, -std::sin(rad), 0},
                    {0, 1, 0, 0},
                    {std::sin(rad), 0, std::cos(rad), 0},
                    {0, 0, 0, 1}
            };

            return mulMatrixVectorR(M, vectors);
        }

    /**Direct rotation around z-axis**/
        static std::vector<Vec3> rotatez(const std::vector<Vec3>& vectors, float degree) {
            float rad = degree * M_PI / 180.0f;

            std::vector<std::vector<float>> M{
                    {std::cos(rad), -std::sin(rad), 0, 0},
                    {std::sin(rad), std::cos(rad), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}
            };

            return mulMatrixVectorR(M, vectors);
        }


//-----------------------------------------

    static void readOBJ(const std::string& name, std::vector<Vec3>& vertices, std::vector<unsigned int>& indices) {
        std::ifstream objFile("./Objs/" + name + ".obj");
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
                lineStream >> vertex.values[0] >> vertex.values[1] >> vertex.values[2];
                vertices.push_back(vertex);
            } else if (prefix == "f") { //read if the first is an 'f'
                char nextChar;
                for (int i = 0; i < 3; ++i) {
                    unsigned int index=0;
                    while (!isspace(lineStream.peek())) {
                        lineStream.get();
                    }
                    while (isspace(lineStream.peek())) {
                        lineStream.get();
                    }
                    while (isdigit(lineStream.peek())) {
                        lineStream >> nextChar;
                        index = index * 10 + (nextChar - '0');
                        std::cout<<index;
                    }
                    std::cout<<"   ";
                    indices.push_back(index - 1); // obj indexes start at 1 so we subtract 1
                }
                std::cout<<std::endl;
            }

        }
        objFile.close();
        /*//PRINT
        for (const auto& v : vertices) {std::cout << "v " << v.x() << " " << v.y() << " " << v.z() << std::endl;}
        for (size_t i = 0; i < indices.size(); i += 3) {
            std::cout << "f "<< indices[i] << " "<< indices[i + 1] << " "<< indices[i + 2] << std::endl;}
        */
    }

    static void normalizeObject(std::vector<Vec3>& vertices) {
        if (vertices.empty()) return;

        // Max, Min values for x,y,z
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

        // Calculate the scale we are looking for
        float maxXRange = maxVertex.x() - minVertex.x();
        float maxYRange = maxVertex.y() - minVertex.y();
        float maxZRange = maxVertex.z() - minVertex.z();

        float maxRange = std::max(maxXRange, std::max(maxYRange, maxZRange));
        float scale = 1.0f / maxRange;

        // Middle point of the bounding box
        Vec3 midPoint(
                (minVertex.x() + maxVertex.x()) / 2.0f,
                (minVertex.y() + maxVertex.y()) / 2.0f,
                (minVertex.z() + maxVertex.z()) / 2.0f
        );

        // Normalize
        for (auto& vertex : vertices) {
            vertex.x((vertex.x() - midPoint.x()) * scale);
            vertex.y((vertex.y() - midPoint.y()) * scale);
            vertex.z((vertex.z() - midPoint.z()) * scale);
        }
    }

};




