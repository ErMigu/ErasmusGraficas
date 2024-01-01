/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"


class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    void loadGeometry();
    virtual void display() override;

    std::array<glm::vec4, 2> getNormalizationPoint(const std::vector<glm::vec4>& vertices);
    void modMat(const std::vector<std::vector<float>> &m, std::string nameMat);
    void resetMatrix();
    void applyParallelView();
    void applyPerspectiveView();
    void applyMove(float x, float y, float z);
    void fullPrint();

    void DrawGui();
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void LoadTexture(const std::string &, const std::string &filename);
    void ShowTexture();
    void UnshowTexture();

private:
    // Program
    GLuint program;

    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;

    // OpenGL attribute locations
    GLuint locVertices;
    GLuint locNormals;
    GLuint locModel;
    GLuint locProjection;
    GLuint locView;
    GLuint locLightPos;
    GLuint locLightColor;
    GLuint locMaterialAmbient;
    GLuint locMaterialDiffuse;
    GLuint locMaterialSpecular;
    GLuint locMaterialShininess;
    GLuint locTexture;
    void debugShader(void) const;

    // Geometry data
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec2> textcoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> indicesN;

    // GuiVar
    std::string objFileName;
    std::string objFilePath;
    std::string textureFileName;
    std::string textureFilePath;
    bool textureShow = false;
    int proj_current_idx = 0;

    // For the mouse camera rotation
    double xMouse=0;
    double yMouse=0;
    float yaw = -90.0f;
    float pitch = 0.0f;
    const float sensibility = 0.1f;
    const float movScale = 0.25f;
    bool mouseActive=false;

    //views
    float fov = 60.0f;
    float aspectRatio = (float) width()/height();
    float obliqueScale = 0;
    float obliqueAngleRad = glm::radians(15.0f);
    float farplane = 500.0f;
    float nearplane = 0.1f;
    float top = 1.0f;
    float right = 0;
    float left = 0;
    float bottom = 0;

    //Lights
    float lightPos[3] = {2.0f, 0.0f, 2.0f};
    float lightColor[3] = {1.0f, 1.0f, 1.0f};
    float ambientColor[3] = {0.2f, 0.2f, 0.2f};

    //Material
    float materialAmbient[3] = {.5f, .5f, .5f};
    float materialDiffuse[3] = {.5f, .5f, .5f};
    float materialSpecular[3] = {.5f, .5f, .5f};
    float materialShininess = 1.0f;

    //camera
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 upVector;

    //mat
    glm::mat4 matModel;
    glm::mat4 P;
    glm::mat4 V;
};
