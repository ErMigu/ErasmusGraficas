/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include "geometryrender.h"
#include "matrixRoutinesAndOBJ.h"

using namespace std;

/**Check if any error has been reported from the shader**/
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

/**Key detection callback**/
void GeometryRender::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        std::string filename;
        switch (key) {
            case GLFW_KEY_O: //change obj case
                std::cout << "Enter the name of the .obj file: ";
                std::cin >> filename;

                matrixRoutinesAndOBJ::readOBJ(filename, vertices, indices);

                loadGeometry();
                break;

            case GLFW_KEY_LEFT: //rotate case
                std::cout << "Left arrow key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::rotate(vertices, 0.0f,10.0f,0.0f, matModel),"matModel");
                break;

            case GLFW_KEY_RIGHT: //rotate case
                std::cout << "Right arrow key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::rotate(vertices, 0.0f, -10.0f, 0.0f, matModel),"matModel");
                break;

            case GLFW_KEY_UP: //rotate case
                std::cout << "Up arrow key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::rotate(vertices, 10.0f, 0.0f, 0.0f, matModel),"matModel");
                break;

            case GLFW_KEY_DOWN: //rotate case
                std::cout << "Down arrow key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::rotate(vertices, -10.0f, 0.0f, 0.0f, matModel),"matModel");
                break;

            case GLFW_KEY_J: //translate case
                std::cout << "J key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::translate(-0.1f, 0.0f, 0.0f),"matModel");
                break;

            case GLFW_KEY_L: //translate case
                std::cout << "L key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::translate(0.1f, 0.0f, 0.0f),"matModel");
                break;

            case GLFW_KEY_I: //translate case
                std::cout << "I key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::translate(0.0f, 0.1f, 0.0f),"matModel");
                break;

            case GLFW_KEY_K: //translate case
                std::cout << "K key pressed." << std::endl;
                modMat(matrixRoutinesAndOBJ::translate(0.0f, -0.1f, 0.0f),"matModel");
                break;

            case GLFW_KEY_E: //Camera positive y-axis
                modMat(matrixRoutinesAndOBJ::translate(0.0f,-0.1f,0.0f),"V");
                break;

            case GLFW_KEY_Q: //Camera negative y-axis
                modMat(matrixRoutinesAndOBJ::translate(0.0f,0.1f,0.0f),"V");
                break;

            case GLFW_KEY_D: //Camera positive x-axis
                modMat(matrixRoutinesAndOBJ::translate(-0.1f,0.0f,0.0f),"V");
                break;

            case GLFW_KEY_A: //Camera negative x-axis
                modMat(matrixRoutinesAndOBJ::translate(0.1f,0.0f,0.0f),"V");
                break;

            case GLFW_KEY_W: //Camera negative z-axis
                modMat(matrixRoutinesAndOBJ::translate(0.0f,0.0f,-0.1f),"V");
                break;

            case GLFW_KEY_S: //Camera positive z-axis
                modMat(matrixRoutinesAndOBJ::translate(0.0f,0.0f,0.1f),"V");
                break;
        }
        if (proj_current_idx == 0) {
            //applyPerspectiveView();
        }
        if (proj_current_idx == 1) {
            //applyParallelView();
        }
        display();
    }
}


//--------------------------------------------------------


/**Initialize OpenGL**/
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

    cameraPos = glm::vec3(0.0f, 0.0f, 3.0);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 aux = glm::lookAt(cameraPos,cameraTarget,upVector);

    std::cout << "V";
    std::cout << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << aux[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;std::cout << std::endl;

    modMat(matrixRoutinesAndOBJ::glmToVec(glm::lookAt(cameraPos,cameraTarget,upVector)),"V");

    locModel = glGetUniformLocation(program,"M");
    locProjection = glGetUniformLocation(program,"P");
    locView = glGetUniformLocation(program,"V");

    loadGeometry();
}

/**Only to load OBJs**/
void GeometryRender::loadGeometry(void)
{
    resetMatrix("all");
    if(vertices.empty()){
        matrixRoutinesAndOBJ::readOBJ("cube.obj",vertices,indices);
    }

    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(Vec3);
    size_t iSize = indices.size()*sizeof(unsigned int);
    glBufferData( GL_ARRAY_BUFFER, vSize, vertices.data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    glUseProgram(0);

    display();
}

/**Render object and display**/
void GeometryRender::display()
{
    glUseProgram(program);

    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel);
    glUniformMatrix4fv(locProjection, 1, GL_TRUE, P);
    glUniformMatrix4fv(locView, 1, GL_TRUE, V);

    glBindVertexArray(vao);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();

    fullPrint();

    glBindVertexArray(0);
    glUseProgram(0);
}


//--------------------------------------------------------


/**Gui management**/
void GeometryRender::DrawGui()
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context.");
    static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
    static ImGuiFileDialog fileDialog;

    ImGui::Begin("3D Studio");
    if (ImGui::CollapsingHeader("OBJ File")) {
        ImGui::Text("OBJ file: %s", objFileName.c_str());
        if (ImGui::Button("Open File"))
            fileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", "./obj");

        if (fileDialog.Display("ChooseFileDlgKey")) {
            if (fileDialog.IsOk() == true) {
                objFileName = fileDialog.GetCurrentFileName();
                objFilePath = fileDialog.GetCurrentPath();
                cout << "OBJ file: " << objFileName << endl << "Path: " << objFilePath << endl;

                resetMatrix("all");
                matrixRoutinesAndOBJ::readOBJ(objFileName, vertices, indices);

                loadGeometry();
                display();
            }
            fileDialog.Close();
        }
    }

    if (ImGui::CollapsingHeader("Projection")) {
        const char* items[] = {"Perspective", "Parallel" };
        if (ImGui::Combo("projektion", &proj_current_idx, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)));
        if (proj_current_idx == 0) {
            ImGui::SliderFloat("Field of view",&fov, 20.0f, 160.0f, "%1.0f", flags);
            ImGui::SliderFloat("Far",&farplane, 1.0f, 1000.0f, "%1.0f", flags);

            applyPerspectiveView();
        }
        if (proj_current_idx == 1) {
            ImGui::SliderFloat("Top",&top, 1.0f, 100.0f, "%.1f", flags);
            ImGui::SliderFloat("Far",&farplane, 1.0f, 1000.0f, "%1.0f", flags);
            ImGui::SliderFloat("Oblique scale",&obliqueScale, 0.0f, 1.0f, "%.1f", flags);
            ImGui::SliderAngle("Oblique angle",&obliqueAngleRad, 15, 75, "%1.0f", flags);

            applyParallelView();
        }
    }
    ImGui::End();
}

/**Update the view and display it**/
void GeometryRender::applyParallelView(){
    resetMatrix("P");
    float right=top*aspectRatio;
    float left=-right;
    float bottom=-top;
    glm::mat4 st=glm::ortho(left, right, bottom, top, farplane, nearplane);
    std::vector<std::vector<float>> ST=matrixRoutinesAndOBJ::glmToVec(st);
    std::vector<std::vector<float>> H = {{
            {1, 0, obliqueScale * std::cos(obliqueAngleRad), 0},
            {0, 1, obliqueScale * std::sin(obliqueAngleRad), 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}}};
    std::vector<std::vector<float>> Paux =matrixRoutinesAndOBJ::mulMatrix4x4(ST,H);

    modMat(Paux,"P");
    display();
}

/**Update the view and display it**/
void GeometryRender::applyPerspectiveView(){
    resetMatrix("P");
    std::cout << fov << "  " << aspectRatio << "  " << nearplane << "  " << farplane<<std::endl;
    glm::mat4 mat=glm::perspective(glm::radians(fov), aspectRatio, nearplane, farplane);
    std::vector<std::vector<float>> Paux=matrixRoutinesAndOBJ::glmToVec(mat);

    modMat(Paux,"P");
    display();
}


//--------------------------------------------------------


/**Used to apply a modification to the filter matrix**/
void GeometryRender::modMat(const std::vector<std::vector<float>>& m, std::string nameMat) {
    if(nameMat=="matModel"){
        //mat4 -> array<array<float>>
        std::vector<std::vector<float>> matAux{
                {matModel[0], matModel[1], matModel[2], matModel[3]},
                {matModel[4], matModel[5], matModel[6], matModel[7]},
                {matModel[8], matModel[9], matModel[10], matModel[11]},
                {matModel[12], matModel[13], matModel[14], matModel[15]}
        };

        //multiply
        std::vector<std::vector<float>> result = matrixRoutinesAndOBJ::mulMatrix4x4(m, matAux);

        //array<array<float>> -> mat4
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                matModel[i * 4 + j] = result[i][j];
            }
        }
    }else{
        if(nameMat=="V"){
            //mat4 -> array<array<float>>
            std::vector<std::vector<float>> matAux{
                    {V[0], V[1], V[2], V[3]},
                    {V[4], V[5], V[6], V[7]},
                    {V[8], V[9], V[10], V[11]},
                    {V[12], V[13], V[14], V[15]}
            };

            //multiply
            std::vector<std::vector<float>> result = matrixRoutinesAndOBJ::mulMatrix4x4(m, matAux);

            //array<array<float>> -> mat4
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    V[i * 4 + j] = result[i][j];
                }
            }
        }else{
            if(nameMat=="P"){
                //mat4 -> array<array<float>>
                std::vector<std::vector<float>> matAux{
                        {P[0], P[1], P[2], P[3]},
                        {P[4], P[5], P[6], P[7]},
                        {P[8], P[9], P[10], P[11]},
                        {P[12], P[13], P[14], P[15]}
                };

                //multiply
                std::vector<std::vector<float>> result = matrixRoutinesAndOBJ::mulMatrix4x4(m, matAux);

                //array<array<float>> -> mat4
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        P[i * 4 + j] = result[i][j];
                    }
                }
            }
        }
    }
}

/**Reset the filter matrix completely**/
void GeometryRender::resetMatrix(std::string name) {
    if(name=="all"){
        for (int i = 0; i < 16; ++i) {
            if (i % 5 == 0) {
                matModel[i] = 1.0f;
                P[i] = 1.0f;
            } else {
                matModel[i] = 0.0f;
                P[i] = 0.0f;
            }
        }
        //modMat(matrixRoutinesAndOBJ::glmToVec(glm::lookAt(cameraPos,cameraTarget,upVector)),"V");
    }else{
        if(name=="matModel"){
            for (int i = 0; i < 16; ++i) {
                if (i % 5 == 0) {
                    matModel[i] = 1.0f;
                } else {
                    matModel[i] = 0.0f;
                }
            }
        }else{
            if(name=="V") {
                //modMat(matrixRoutinesAndOBJ::glmToVec(glm::lookAt(cameraPos,cameraTarget,upVector)),"V");
            }else{
                if(name=="P"){
                    for (int i = 0; i < 16; ++i) {
                        if (i % 5 == 0) {
                            P[i] = 1.0f;
                        } else {
                            P[i] = 0.0f;
                        }
                    }
                }
            }
        }
    }
}

/**Give the 2 corners of the cube that contains the obj**/
std::array<Vec3, 2> GeometryRender::getNormalizationPoint(const std::vector<Vec3>& vertices){
    // Inicializa los puntos con el primer vértice como punto de partida
    Vec3 left_bottom_near = vertices[0];
    Vec3 right_top_far = vertices[0];

    for (const auto& vertexDefault : vertices) {
        Vec3 vertex= matrixRoutinesAndOBJ::apply3Matrix(vertexDefault,matModel);
        // Encuentra el punto más a la izquierda, más abajo y más cercano
        if (vertex.x() < left_bottom_near.x()) left_bottom_near.x(vertex.x());
        if (vertex.y() < left_bottom_near.y()) left_bottom_near.y(vertex.y());
        if (vertex.z() < left_bottom_near.z()) left_bottom_near.z(vertex.z());

        // Encuentra el punto más a la derecha, más arriba y más lejano
        if (vertex.x() > right_top_far.x()) right_top_far.x(vertex.x());
        if (vertex.y() > right_top_far.y()) right_top_far.y(vertex.y());
        if (vertex.z() > right_top_far.z()) right_top_far.z(vertex.z());
    }
    return {left_bottom_near, right_top_far};
}

void GeometryRender::fullPrint(){
    std::cout << "MATMODEL";
    std::cout << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << matModel[i*4+j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "P";
    std::cout << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << P[i*4+j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "V";
    std::cout << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << V[i*4+j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;std::cout << std::endl;

    /*
    std::cout << std::endl;
    for (int i=0; i<vertices.size(); i++) {
        std::cout << vertices[i].x() << " " << vertices[i].y() << " " << vertices[i].z() << std::endl;
    }
    std::cout << std::endl;std::cout << std::endl;*/
}

//--------------------------------------------------------
