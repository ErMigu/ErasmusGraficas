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
    glm::vec3 mov;
    if (action == GLFW_PRESS) {
        std::string filename;
        switch (key) {
            case GLFW_KEY_O: //change obj case
                std::cout << "Enter the name of the .obj file: ";
                std::cin >> filename;

                matrixRoutinesAndOBJ::readOBJ(filename, vertices, normals, indices, indicesN);

                loadGeometry();
                break;

            case GLFW_KEY_LEFT: //rotate case
                std::cout << "Left arrow key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::rotate(0.0f,-10.0f,0.0f, matModel);
                break;

            case GLFW_KEY_RIGHT: //rotate case
                std::cout << "Right arrow key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::rotate(0.0f, 10.0f, 0.0f, matModel);
                break;

            case GLFW_KEY_UP: //rotate case
                std::cout << "Up arrow key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::rotate(-10.0f, 0.0f, 0.0f, matModel);
                break;

            case GLFW_KEY_DOWN: //rotate case
                std::cout << "Down arrow key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::rotate(10.0f, 0.0f, 0.0f, matModel);
                break;

            case GLFW_KEY_J: //translate case
                std::cout << "J key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::translate(-0.1f, 0.0f, 0.0f)*matModel;
                break;

            case GLFW_KEY_L: //translate case
                std::cout << "L key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::translate(0.1f, 0.0f, 0.0f)*matModel;
                break;

            case GLFW_KEY_I: //translate case
                std::cout << "I key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::translate(0.0f, 0.1f, 0.0f)*matModel;
                break;

            case GLFW_KEY_K: //translate case
                std::cout << "K key pressed." << std::endl;
                matModel=matrixRoutinesAndOBJ::translate(0.0f, -0.1f, 0.0f)*matModel;
                break;

            case GLFW_KEY_E: // Cámara eje y positivo
                mov = glm::vec3(0.0, movScale, 0.0);
                cameraTarget += mov; // Mueve el objetivo de la cámara
                cameraPos += mov; // Actualiza la posición de la cámara
                V = glm::lookAt(cameraPos, cameraTarget, upVector);
                break;

            case GLFW_KEY_Q: // Cámara eje y negativo
                mov = glm::vec3(0.0, -movScale, 0.0);
                cameraTarget += mov; // Mueve el objetivo de la cámara
                cameraPos += mov; // Actualiza la posición de la cámara
                V = glm::lookAt(cameraPos, cameraTarget, upVector);
                break;

            case GLFW_KEY_W: // Mover hacia adelante
                mov = glm::normalize(cameraTarget - cameraPos)*movScale;
                cameraTarget += mov; // Mueve el objetivo de la cámara
                cameraPos += mov; // Actualiza la posición de la cámara
                V = glm::lookAt(cameraPos, cameraTarget, upVector);
                break;

            case GLFW_KEY_S: // Mover hacia atrás
                mov = -glm::normalize(cameraTarget - cameraPos)*movScale;
                cameraTarget += mov; // Mueve el objetivo de la cámara
                cameraPos += mov; // Actualiza la posición de la cámara
                V = glm::lookAt(cameraPos, cameraTarget, upVector);
                break;

            case GLFW_KEY_A: // Mover hacia la izquierda
                mov = -glm::normalize(glm::cross(cameraTarget - cameraPos, upVector))*movScale;
                cameraTarget += mov; // Mueve el objetivo de la cámara
                cameraPos += mov; // Actualiza la posición de la cámara
                V = glm::lookAt(cameraPos, cameraTarget, upVector);
                break;

            case GLFW_KEY_D: // Mover hacia la derecha
                mov = glm::normalize(glm::cross(cameraTarget - cameraPos, upVector))*movScale;
                cameraTarget += mov; // Mueve el objetivo de la cámara
                cameraPos += mov; // Actualiza la posición de la cámara
                V = glm::lookAt(cameraPos, cameraTarget, upVector);
                break;
        }
        display();
    }
}

/**Mouse detection callback**/
void GeometryRender::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        if(mouseActive == true) {
            float deltaX = (xpos - xMouse) * sensibility; float deltaY = (ypos - yMouse) * sensibility;

            yaw += deltaX;
            pitch -= deltaY;
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

            cameraTarget = cameraPos + glm::normalize(front);
            V = glm::lookAt(cameraPos, cameraTarget, upVector);
            display();
        }else{
            mouseActive=true;
        }
        xMouse = xpos;
        yMouse = ypos;
    }else{ //si dejo de pulsar, pongo el mouse en nada
        mouseActive=false;
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

    //Get matrix locations for each matrix
    locModel = glGetUniformLocation(program,"M");
    locProjection = glGetUniformLocation(program,"P");
    locView = glGetUniformLocation(program,"V");
    locNormals = glGetAttribLocation(program, "vNormal");
    locLightPos = glGetUniformLocation(program, "lightPos");
    locLightColor = glGetUniformLocation(program, "lightColor");
    locMaterialAmbient = glGetUniformLocation(program, "materialAmbient");
    locMaterialDiffuse = glGetUniformLocation(program, "materialDiffuse");
    locMaterialSpecular = glGetUniformLocation(program, "materialSpecular");
    locMaterialShininess = glGetUniformLocation(program, "materialShininess");

    //Initializes matrixes
    matModel=glm::mat4(1.0f);
    P=glm::mat4(1.0f);
    V=glm::mat4(1.0f);

    //Initializes the camera
    cameraPos = glm::vec3(0.0f, 0.0f, 1.0);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    V=glm::lookAt(cameraPos,cameraTarget,upVector);

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

/**Only to load OBJs**/
void GeometryRender::loadGeometry(void)
{
    matModel=glm::mat4(1.0f);
    if(vertices.empty()){
        matrixRoutinesAndOBJ::readOBJ("sphere.obj",vertices,normals,indices,indicesN);
    }

    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(glm::vec4);
    size_t iSize = indices.size()*sizeof(unsigned int);
    glBufferData( GL_ARRAY_BUFFER, vSize, vertices.data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );

    // Cargar datos de normales
    size_t nSize = normals.size() * sizeof(glm::vec4);
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, nSize, normals.data(), GL_STATIC_DRAW);

    // Configurar atributos de normales
    glVertexAttribPointer(locNormals, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(locNormals);

    glBindVertexArray(0);
    glUseProgram(0);

    display();
}

/**Render object and display**/
void GeometryRender::display()
{
    glUseProgram(program);
    glBindVertexArray(vao);

    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(V));

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Uniform for lights and materials
    glUniform3fv(locLightPos, 1, lightPos);
    glUniform3fv(locLightColor, 1, lightColor);
    glUniform3fv(locMaterialAmbient, 1, materialAmbient);
    glUniform3fv(locMaterialDiffuse, 1, materialDiffuse);
    glUniform3fv(locMaterialSpecular, 1, materialSpecular);
    glUniform1f(locMaterialShininess, materialShininess);

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
    static ImGuiFileDialog textureDialog;
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

                matrixRoutinesAndOBJ::readOBJ(objFileName, vertices, normals, indices, indicesN);

                loadGeometry();
                display();
            }
            fileDialog.Close();
        }
    }

    if (ImGui::CollapsingHeader("Light")) { //TODO
        ImGui::Text("Light source position");
        ImGui::PushItemWidth(100);
        ImGui::InputFloat("x", &lightPos[0], 0.5f, 1.0f, "%1.1f"); ImGui::SameLine();
        ImGui::InputFloat("y", &lightPos[1], 0.5f, 1.0f, "%1.1f"); ImGui::SameLine();
        ImGui::InputFloat("z", &lightPos[2], 0.5f, 1.0f, "%1.1f");
        ImGui::PopItemWidth();

        ImGui::Text("Light source intensity:");
        ImGui::ColorEdit3("Light", lightColor);

        ImGui::Text("Ambient light intensity:");
        ImGui::ColorEdit3("Ambient", ambientColor);
    }

    if (ImGui::CollapsingHeader("Object Material")) { //TODO
        ImGui::Text("Ambient coefficient:");
        ImGui::ColorEdit3("Ambient color", materialAmbient);

        ImGui::Text("Diffuse coefficient:");
        ImGui::ColorEdit3("Diffuse color", materialDiffuse);

        ImGui::Text("Specular coefficient:");
        ImGui::ColorEdit3("Specular color", materialSpecular);

        ImGui::SliderFloat("Shininess", &materialShininess, 1.0f, 1000.0f, "%1.0f", flags);
    }

    if (ImGui::CollapsingHeader("Object Texture")) { //TODO
        ImGui::Checkbox("Show texture", &textureShow);
        ImGui::Text("Texture file: %s", textureFileName.c_str());
        if (ImGui::Button("Open Texture File"))
            textureDialog.OpenDialog("ChooseFileDlgKey", "Choose Texture File",
                                     ".jpg,.bmp,.dds,.hdr,.pic,.png,.psd,.tga", ".");

        if (textureDialog.Display("ChooseFileDlgKey")) {
            if (textureDialog.IsOk() == true) {
                textureFileName = textureDialog.GetCurrentFileName();
                textureFilePath = textureDialog.GetCurrentPath();
                cout << "Texture file: " << textureFileName << endl << "Path: " << textureFilePath << endl;
            } else {
                // Return a message to the user if the file could not be opened
            }
            // close
            textureDialog.Close();
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
void GeometryRender::applyPerspectiveView(){
    glm::mat4 mat=glm::perspective(glm::radians(fov), aspectRatio, nearplane, farplane);

    P=mat;
    display();
}

/**Update the view and display it**/
void GeometryRender::applyParallelView(){
    aspectRatio = (float) width()/height();
    right = top * aspectRatio;
    left = -right;
    bottom = -top;
    glm::mat4 st=glm::ortho(left, right, bottom, top, nearplane, farplane);

    if(obliqueScale > 0.0f){
        glm::mat4 H = glm::mat4(1.0f);
        H[2][0] = obliqueScale * cos(obliqueAngleRad);
        H[2][1] = obliqueScale * sin(obliqueAngleRad);
        P=st*H*matrixRoutinesAndOBJ::translate(0.0f, obliqueScale*3, 0.0f)*matrixRoutinesAndOBJ::translate(obliqueScale*3, 0.0f, 0.0f);
    }else{
        P=st;
    }

    display();
}


//--------------------------------------------------------


/**Full print of the matrix (they are all already transposed)**/
void GeometryRender::fullPrint(){
    /*
    std::cout << "MATMODEL";
    std::cout << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << matModel[i][j] << " ";
        }
        std::cout << std::endl;
    }


    std::cout << "P";
    std::cout << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << P[i][j] << " ";
        }
        std::cout << std::endl;
    }


    std::cout << "V";
    std::cout << std::endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            std::cout << V[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;std::cout << std::endl;


    std::cout << std::endl;
    for (int i=0; i<vertices.size(); i++) {
        glm::vec4 transformed_vertex = P * V * matModel * vertices[i];
        std::cout << transformed_vertex.x << " " << transformed_vertex.y << " " << transformed_vertex.z << std::endl;
    }
    std::cout << std::endl;std::cout << std::endl;
    */
}


//--------------------------------------------------------
