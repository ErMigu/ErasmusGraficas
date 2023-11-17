/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include "geometryrender.h"
#include "matrixRoutinesAndOBJ.h"

using namespace std;


// Initialize OpenGL
void GeometryRender::initialize()
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Create and initialize a program object with shaders
    program = initProgram("vshader.glsl", "fshader.glsl");
    // Installs the program object as part of current rendering state
    glUseProgram(program);

    // Creat a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vertex buffer in the shared display list space and
    // bind it as VertexBuffer (GL_ARRAY_BUFFER)
    glGenBuffers( 1, &vBuffer);
    glBindBuffer( GL_ARRAY_BUFFER, vBuffer);

    /* Create buffer in the shared display list space and 
       bind it as GL_ELEMENT_ARRAY_BUFFER */
    glGenBuffers(1, &iBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

    // Get locations of the attributes in the shader
    locVertices = glGetAttribLocation( program, "vPosition");

    glBindVertexArray(0);
    glUseProgram(0);

    loadGeometry();
}


void GeometryRender::loadGeometry(void)
{
    if(vertices.empty()){ //at first the sphere is loaded
        matrixRoutinesAndOBJ::readOBJ("sphere",vertices,indices);
    }
    if(newObject){ //we only normalize objects that are loaded, not rotation or translation
        matrixRoutinesAndOBJ::normalizeObject(vertices);
    }


    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    locModel = glGetUniformLocation(program,"M");
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(Vec3);
    size_t iSize = indices.size()*sizeof(unsigned int);
    glBufferData( GL_ARRAY_BUFFER, vSize, vertices.data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    glUseProgram(0);
}


// Check if any error has been reported from the shader
void GeometryRender::debugShader(void) const
{
    GLint  logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize );
    if (logSize > 0) {
        std::cerr << "Failure in shader "  << std::endl;
        char logMsg[logSize+1];
        glGetProgramInfoLog( program, logSize, nullptr, &(logMsg[0]) );
        std::cerr << "Shader info log: " << logMsg << std::endl;
    }

}


// Render object
void GeometryRender::display()
{
    glUseProgram(program);
    glBindVertexArray(vao);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();

    glBindVertexArray(0);
    glUseProgram(0);
}


// GLFW key callback function
void GeometryRender::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        std::string filename;
        switch (key) {
            case GLFW_KEY_O: //change obj case
                std::cout << "Enter the name of the .obj file: ";
                std::cin >> filename;

                vertices.clear();
                indices.clear();

                matrixRoutinesAndOBJ::readOBJ(filename, vertices, indices);

                this->newObject=true; //change to new object mode
                loadGeometry();
                display();
                break;

            case GLFW_KEY_LEFT: //rotate case
                std::cout << "Left arrow key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::rotatey(vertices, 10.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;

            case GLFW_KEY_RIGHT: //rotate case
                std::cout << "Right arrow key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::rotatey(vertices, -10.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;

            case GLFW_KEY_UP: //rotate case
                std::cout << "Up arrow key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::rotatex(vertices, 10.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;

            case GLFW_KEY_DOWN: //rotate case
                std::cout << "Down arrow key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::rotatex(vertices, -10.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;

            case GLFW_KEY_J: //translate case
                std::cout << "J key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::translate(vertices, -0.1f, 0.0f, 0.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;

            case GLFW_KEY_L: //translate case
                std::cout << "L key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::translate(vertices, 0.1f, 0.0f, 0.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;

            case GLFW_KEY_I: //translate case
                std::cout << "I key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::translate(vertices, 0.0f, 0.1f, 0.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;

            case GLFW_KEY_K: //translate case
                std::cout << "K key pressed." << std::endl;
                vertices = matrixRoutinesAndOBJ::translate(vertices, 0.0f, -0.1f, 0.0f);
                this->newObject=false;
                loadGeometry();
                display();
                break;
        }
    }
}