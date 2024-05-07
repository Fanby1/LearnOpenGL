#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CWindow.h"
#include "CShaderFrame.h"
#include "CShader.h"

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

static unsigned int linkShader(unsigned int vVertexShader, unsigned int vFragmentShader) {
    // link shaders
    unsigned int ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, vVertexShader);
    glAttachShader(ShaderProgram, vFragmentShader);
    glLinkProgram(ShaderProgram);
    // check for linking errors
    int Success;
    char InfoLog[512];;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
    }
    glDeleteShader(vVertexShader);
    glDeleteShader(vFragmentShader);
    return ShaderProgram;
}

static unsigned int createVBO(float* vVertices, size_t vSize) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vSize, vVertices, GL_STATIC_DRAW);
    return VBO;
}

static unsigned int createEBO(unsigned int* vIndices, size_t vSize) {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vSize, vIndices, GL_STATIC_DRAW);
    return EBO;
}

static unsigned int createVAO(unsigned int VBO, unsigned int EBO) {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    return VAO;
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

    auto VBO = createVBO(Vertices, sizeof(Vertices));
    auto EBO_1 = createEBO(IndicesFirst, sizeof(IndicesFirst));
    auto EBO_2 = createEBO(IndicesSecond, sizeof(IndicesSecond));
    auto VAO_1 = createVAO(VBO, EBO_1);
    auto VAO_2 = createVAO(VBO, EBO_2);
   
    CShader OrangeShader("./Shader/vertex.vs", "./Shader/orange.fs");
    CShader YelloShader("./Shader/vertex.vs", "./Shader/yellow.fs");
    
    Window.render({ OrangeShader.ID, YelloShader.ID }, { VAO_1 , VAO_2});
    //Window.render({ ShaderProgrameOrange }, { VAO_1 });
}