/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "3dstudio.h"

const float pi_f = 3.1415926f;

class OpenGLWindow
{
public:
    OpenGLWindow(std::string title, int width, int height);
    ~OpenGLWindow();

    GLFWwindow* window() const;
    virtual void errorCallback(int error, const char* desc);
    virtual void resizeCallback(GLFWwindow* window, int width, int height);
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0; //I will code it in geometryrender
                                                                                                //to have access to its methods

    void start();
    virtual void initialize() = 0;
    virtual void display() = 0;
    void displayNow();

protected:
    bool checkOpenGLError() const;
    int width() const;
    int height() const;

    std::string readShaderSource(const std::string shaderFile) const;
    GLuint initProgram(const std::string vShaderFile, const std::string fShaderFile) const;

    void reshape(const int width, const int height) const;

    int windowWidth = 0;
    int windowHeight = 0;
    float aspectRatio = 0;

private:
    virtual void DrawGui()=0;

    GLFWwindow* glfwWindow;
};
