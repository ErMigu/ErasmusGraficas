/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"

typedef float Mat4x4[16];

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    void loadgeometry();
    virtual void display() override;

    std::array<Vec3, 2> getNormalizationPoint(const std::vector<Vec3>& vertices);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void modMat(const std::vector<std::vector<float>> &m, std::string nameMat);
    void resetMatrix(std::string name);
    void applyParallelView();
    void applyPerspectiveView();

    void DrawGui();

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
    void loadGeometry(void);

    // Geometry data
    std::vector<Vec3> vertices;
    std::vector<unsigned int> indices;

    // GuiVar
    std::string objFileName;
    std::string objFilePath;

    float fov = 60.0f;
    float farplane = 500.0f;
    float top = 1.0f;
    float obliqueScale = 0.0f;
    float obliqueAngleRad = pi_f/4.0f;

    //for the gui
    int proj_current_idx = 0;

    Mat4x4 matModel = {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0, 1.0, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f};

    Mat4x4 V = {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0, 1.0, -2.0f,
                       0.0f, 0.0f, 0.0f, 1.0f};

    Mat4x4 P = {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0, 1.0, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f};

};
