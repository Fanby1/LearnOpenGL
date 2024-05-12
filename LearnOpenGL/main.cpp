#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CWindow.h"
#include "CShader.h"
#include "CObject.h"
#include "CTexture.h"
#include "CVertexArrayObject.h"
#include "CVertexBufferObject.h"
#include "CElementBufferObject.h"

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
    auto OrangeShader = std::make_shared<CShader>("./Shader/vertex.vs", "./Shader/orange.fs");
    auto YelloShader = std::make_shared<CShader>("./Shader/vertex.vs", "./Shader/yellow.fs");
    
    std::vector<unsigned int> temp = { 3u };
    auto VBO = std::make_shared<CVertexBufferObject>(Vertices, sizeof(Vertices), VERTEX_TYPE_VERTEX_BIT, temp);
    auto EBOYellow = std::make_shared<CElementBufferObject>(IndicesSecond, sizeof(IndicesSecond));
    auto EBOOrange = std::make_shared<CElementBufferObject>(IndicesFirst, sizeof(IndicesFirst));

    auto VAOYellow = std::make_shared<CVertexArrayObject>(), VAOOrange = std::make_shared<CVertexArrayObject>();
    VAOYellow->addVBO(VBO);
    VAOYellow->setEBO(EBOYellow);
    
    VAOOrange->addVBO(VBO);
    VAOOrange->setEBO(EBOOrange);
    auto TriangleOrange = std::make_shared<CObject>(), TriangleYellow = std::make_shared<CObject>();
    TriangleOrange->addVAO(VAOOrange, OrangeShader);
    TriangleYellow->addVAO(VAOYellow, YelloShader);

    vWindow.setObject({ TriangleOrange, TriangleYellow });
    
    vWindow.render();
    
}
/*
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
*/

int main() {
    initGLFW();
    CWindow Window(800, 600);
    initGLAD();
    
    renderOrangeAndYellow(Window);
    // renderMix(Window);
    // renderTexture(Window);
}