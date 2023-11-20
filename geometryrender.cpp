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
    resetMatModel();

    if(vertices.empty()){ //at first the sphere is loaded
        matrixRoutinesAndOBJ::readOBJ("sphere",vertices,indices);
        matrixRoutinesAndOBJ::normalizeObject(vertices);
    }else{
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

    locModel = glGetUniformLocation(program,"M");
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel);

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

                loadGeometry();
                display();
                break;

            case GLFW_KEY_LEFT: //rotate case
                std::cout << "Left arrow key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::rotatey(vertices, 10.0f, matModel));
                display();
                break;

            case GLFW_KEY_RIGHT: //rotate case
                std::cout << "Right arrow key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::rotatey(vertices, -10.0f, matModel));
                display();
                break;

            case GLFW_KEY_UP: //rotate case
                std::cout << "Up arrow key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::rotatex(vertices, 10.0f, matModel));
                display();
                break;

            case GLFW_KEY_DOWN: //rotate case
                std::cout << "Down arrow key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::rotatex(vertices, -10.0f, matModel));
                display();
                break;

            case GLFW_KEY_J: //translate case
                std::cout << "J key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::translate(-0.1f, 0.0f, 0.0f));
                display();
                break;

            case GLFW_KEY_L: //translate case
                std::cout << "L key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::translate(0.1f, 0.0f, 0.0f));
                display();
                break;

            case GLFW_KEY_I: //translate case
                std::cout << "I key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::translate(0.0f, 0.1f, 0.0f));
                display();
                break;

            case GLFW_KEY_K: //translate case
                std::cout << "K key pressed." << std::endl;
                modMatModel(matrixRoutinesAndOBJ::translate(0.0f, -0.1f, 0.0f));
                display();
                break;
        }
    }
}

void GeometryRender::modMatModel(const std::vector<std::vector<float>>& m) {
    //mat4 -> array<array<float>>
    std::vector<std::vector<float>> matModelVec{
            {matModel[0], matModel[1], matModel[2], matModel[3]},
            {matModel[4], matModel[5], matModel[6], matModel[7]},
            {matModel[8], matModel[9], matModel[10], matModel[11]},
            {matModel[12], matModel[13], matModel[14], matModel[15]}
    };

    //multiply
    std::vector<std::vector<float>> result = matrixRoutinesAndOBJ::mulMatrix4x4(m, matModelVec);

    //array<array<float>> -> mat4
    matModel[0] = result[0][0];
    matModel[1] = result[0][1];
    matModel[2] = result[0][2];
    matModel[3] = result[0][3];
    matModel[4] = result[1][0];
    matModel[5] = result[1][1];
    matModel[6] = result[1][2];
    matModel[7] = result[1][3];
    matModel[8] = result[2][0];
    matModel[9] = result[2][1];
    matModel[10] = result[2][2];
    matModel[11] = result[2][3];
    matModel[12] = result[3][0];
    matModel[13] = result[3][1];
    matModel[14] = result[3][2];
    matModel[15] = result[3][3];
}


void GeometryRender::resetMatModel() {
    for (int i = 0; i < 16; ++i) {
        if (i % 5 == 0) {
            matModel[i] = 1.0f;
        } else {
            matModel[i] = 0.0f;
        }
    }
}

