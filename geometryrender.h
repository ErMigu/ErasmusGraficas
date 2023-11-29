/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"
#include <glm/glm.hpp>

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    void loadGeometry();
    virtual void display() override;

    std::array<glm::vec3, 2> getNormalizationPoint(const std::vector<glm::vec3>& vertices);
    void modMat(const std::vector<std::vector<float>> &m, std::string nameMat);
    void resetMatrix(std::string name);
    void applyParallelView();
    void applyPerspectiveView();
    void applyMove(float x, float y, float z);
    void fullPrint();

    void DrawGui();
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);

private:
    // Program
    GLuint program;


    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;


    // OpenGL attribute locations
    GLuint locVertices;
    GLuint locModel;
    GLuint locProjection;
    GLuint locView;

    void debugShader(void) const;


    // Geometry data
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;


    // GuiVar
    std::string objFileName;
    std::string objFilePath;

    float fov = 60.0f;
    float farplane = 100.0f;
    float nearplane = -10;
    float top = 1.0f;
    float obliqueScale = 0.0f;
    float obliqueAngleRad = pi_f/4.0f;

    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 upVector;


    //for the gui
    int proj_current_idx = 0;
    double xMouse=null;
    double yMouse=null;

    glm::mat4 matModel = {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0, 1.0, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f};

    glm::mat4 P = {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0, 1.0, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f};

    glm::mat4 V = {1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0, 1.0, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f};
};
