#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "CWindow.h"
#include "CShader.h"
#include "CObject.h"
#include "CTexture.h"
#include "CVertexArrayObject.h"
#include "CVertexBufferObject.h"
#include "CElementBufferObject.h"
#include "CCamera.h"

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
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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

static void renderMix(CWindow& vWindow) {
    auto MixShader = std::make_shared<CShader>("./Shader/mix.vs", "./Shader/mix.fs");

    std::vector<unsigned int> Offset = { 3,3 };
    unsigned int Index[] = {0,1,2};
    auto MixVBO = std::make_shared<CVertexBufferObject>(MixVertices, sizeof(MixVertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_COLOR_BIT, Offset);
    auto MixEBO = std::make_shared<CElementBufferObject>(Index, sizeof(Index));
    auto MixVAO = std::make_shared<CVertexArrayObject>();

    MixVAO->addVBO(MixVBO);
    MixVAO->setEBO(MixEBO);

    auto Mix = std::make_shared<CObject>();
    Mix->addVAO(MixVAO, MixShader);
    
    vWindow.addObject(Mix);

    vWindow.render();
}

static void renderTexture(CWindow& vWindow) {
    auto TextureShader = std::make_shared<CShader>("./Shader/texture.vs", "./Shader/texture.fs");

    std::vector<unsigned int> Offset = { 3,3,2 };

    auto RectangleVBO = std::make_shared<CVertexBufferObject>(TextureVertices, sizeof(TextureVertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_COLOR_BIT | VERTEX_TYPE_TEXTURE_BIT, Offset);
    auto RectangleEBO = std::make_shared<CElementBufferObject>(TextureIndices, sizeof(TextureIndices));
    auto RectangleVAO = std::make_shared<CVertexArrayObject>();

    RectangleVAO->addVBO(RectangleVBO);
    RectangleVAO->setEBO(RectangleEBO);

    auto Rectangle = std::make_shared<CObject>();
    Rectangle->addVAO(RectangleVAO, TextureShader);

    CImage Image("./Assert/container.jpg");
    CTexture Texture(Image, GL_TEXTURE0);
    Texture.bind();
    
    vWindow.addObject(Rectangle);

    vWindow.render();
}

static void renderTransform(CWindow& vWindow) {
    auto TransformShader = std::make_shared<CShader>("./Shader/transform.vs", "./Shader/transform.fs");
    TransformShader->use();
    TransformShader->setInt("texture1", 0);
    TransformShader->setInt("texture2", 1);

    float Vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int Indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    CImage Container("./Assert/container.jpg");
    CTexture Texture_0(Container, GL_TEXTURE0);
    CImage Awesomeface("./Assert/awesomeface.png");
    CTexture Texture_1(Awesomeface, GL_TEXTURE1);

    Texture_0.bind();
    Texture_1.bind();

    std::vector<unsigned int> Offset = { 3,0,2 };
    auto TransformVBO = std::make_shared<CVertexBufferObject>(Vertices, sizeof(Vertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_TEXTURE_BIT, Offset);
    auto TransformEBO = std::make_shared<CElementBufferObject>(Indices, sizeof(Indices));
    auto TransformVAO = std::make_shared<CVertexArrayObject>();

    TransformVAO->addVBO(TransformVBO);
    TransformVAO->setEBO(TransformEBO);

    auto Transform = std::make_shared<CObject>();
    Transform->addVAO(TransformVAO, TransformShader);

    CCamera Camera;
    Camera.setCameraPosition({ 0, 0, 3});
    // Camera.setCameraTarget({ 0,0,0 });
    Camera.setFarPlane(100);
    Camera.setNearPlane(0.1);
    Camera.setAspectRatio(800.0 / 600.0);
    Camera.setFeildOfView(45.0);
    TransformShader->setProjection(Camera.getProjectionMatrix());
    TransformShader->setView(Camera.getViewMatrix());
    vWindow.addObject(Transform);
    glEnable(GL_DEPTH_TEST);
    vWindow.render();
}


int main() {
    initGLFW();
    CWindow Window(800, 600);
    initGLAD();
    
    // renderOrangeAndYellow(Window);
    // renderMix(Window);
    // renderTexture(Window);
    renderTransform(Window);
}