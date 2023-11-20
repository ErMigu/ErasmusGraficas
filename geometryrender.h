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


    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void modMatModel(const std::vector<std::vector<float>> &m);
    void resetMatModel();

private:
    GLuint program;

    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;

    // OpenGL attribute locations
    GLuint locVertices;
    GLuint locModel;

    void debugShader(void) const;
    void loadGeometry(void);

    // Geometry data
    std::vector<Vec3> vertices;
    std::vector<unsigned int> indices;

    Mat4x4 matModel = {1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0, 1.0, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f};

};
