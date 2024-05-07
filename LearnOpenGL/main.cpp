#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CWindow.h"
#include "CShaderFrame.h"
#include "CShader.h"
#include "CObject.h"

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float Vertices[] = {
 0.5f,  0.5f, 0.0f,  // top right
 0.5f, -0.5f, 0.0f,  // bottom right
-0.5f, -0.5f, 0.0f,  // bottom left
-0.5f,  0.5f, 0.0f   // top left 
};
unsigned int IndicesFirst[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
};

unsigned int IndicesSecond[] = {
     1, 2, 3    // second triangle
};

static void initGLAD() {
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}


static void initGLFW() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main() {
    initGLFW();
    CWindow Window(800, 600);
    initGLAD();
    
    CObject TriangleOrange, TriangleYellow;
    auto VBO = TriangleOrange.createVBO(Vertices, sizeof(Vertices), VERTEX_TYPE_VERTEX_BIT);
    TriangleOrange.createEBO(IndicesFirst, sizeof(IndicesFirst));
    TriangleOrange.createVAO();
    TriangleYellow.createEBO(IndicesSecond, sizeof(IndicesSecond));
    TriangleYellow.addVBO(VBO, VERTEX_TYPE_VERTEX_BIT);
   
    CShader OrangeShader("./Shader/vertex.vs", "./Shader/orange.fs");
    CShader YelloShader("./Shader/vertex.vs", "./Shader/yellow.fs");
    
    Window.render({ OrangeShader.ID, YelloShader.ID }, { TriangleOrange.getVAO() , TriangleYellow.getVAO()});
    //Window.render({ ShaderProgrameOrange }, { VAO_1 });
}