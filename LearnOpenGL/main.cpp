#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CWindow.h"
#include "CShaderFrame.h"
#include "CShader.h"
#include "CObject.h"
#include "CTexture.h"

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float Vertices[] = {
 0.5f,  0.5f, 0.0f,  // top right
 0.5f, -0.5f, 0.0f,  // bottom right
-0.5f, -0.5f, 0.0f,  // bottom left
-0.5f,  0.5f, 0.0f   // top left 
};

float MixVertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

float TextureVertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
};
unsigned int IndicesFirst[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
};

unsigned int IndicesSecond[] = {
     1, 2, 3    // second triangle
};

unsigned int TextureIndices[] = {
     0, 1, 3,   // first triangle
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

static void renderOrangeAndYellow(CWindow& vWindow) {
    CObject TriangleOrange, TriangleYellow;
    auto VBO = TriangleOrange.createVBO(Vertices, sizeof(Vertices), VERTEX_TYPE_VERTEX_BIT, {3});
    TriangleOrange.createEBO(IndicesFirst, sizeof(IndicesFirst));
    TriangleOrange.createVAO();
    TriangleYellow.createEBO(IndicesSecond, sizeof(IndicesSecond));
    TriangleYellow.addVBO(VBO, VERTEX_TYPE_VERTEX_BIT, {3});

    CShader OrangeShader("./Shader/vertex.vs", "./Shader/orange.fs");
    CShader YelloShader("./Shader/vertex.vs", "./Shader/yellow.fs");

    vWindow.render({ OrangeShader.ID, YelloShader.ID }, { TriangleOrange.getVAO() , TriangleYellow.getVAO() });
}

static void renderMix(CWindow& vWindow) {
    CObject Mix;
    Mix.createVBO(MixVertices, sizeof(MixVertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_COLOR_BIT, {3,3});
    unsigned int temp[] = { 0,1,2 };
    Mix.createEBO(temp, sizeof(temp));
    Mix.createVAO();
    
    CShader MixShader("./Shader/mix.vs", "./Shader/mix.fs");

    vWindow.render({ MixShader.ID }, { Mix.getVAO() });
}

static void renderTexture(CWindow& vWindow) {
    CObject Rectangle;
    Rectangle.createVBO(TextureVertices, sizeof(TextureVertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_COLOR_BIT | VERTEX_TYPE_TEXTURE_BIT, { 3,3,2 });
    Rectangle.createEBO(TextureIndices, sizeof(TextureIndices));
    Rectangle.createVAO();

    CImage Image("./Assert/container.jpg");
    CTexture Texture(Image);
    Texture.bind();
    CShader TextureShader("./Shader/texture.vs", "./Shader/texture.fs");

    vWindow.render({ TextureShader.ID }, { Rectangle.getVAO() });
}

int main() {
    initGLFW();
    CWindow Window(800, 600);
    initGLAD();
    
    // renderOrangeAndYellow(Window);
    // renderMix(Window);
    renderTexture(Window);
}