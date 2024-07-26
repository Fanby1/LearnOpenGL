// renderOrangeAndYellow(Window);
// renderMix(Window);
// renderMixSquare(Window);
// renderTexture(Window);
// renderTransform(Window);
// renderCube(Window);
#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CWindow.h"
#include "CShader.h"
#include "CObject.h"
#include "CTexture.h"
#include "CVertexArrayObject.h"
#include "CVertexBufferObject.h"
#include "CElementBufferObject.h"
#include "CCamera.h"
#include "HiveLogger.h"

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
    auto TriangleOrange = std::make_shared<CStuff>(), TriangleYellow = std::make_shared<CStuff>();
    TriangleOrange->addVAO(VAOOrange, OrangeShader);
    TriangleYellow->addVAO(VAOYellow, YelloShader);

    vWindow.setStuff({ TriangleOrange, TriangleYellow });

    vWindow.render();

}

static void renderMix(CWindow& vWindow) {
    auto MixShader = std::make_shared<CShader>("./Shader/mix.vs", "./Shader/mix.fs");

    std::vector<unsigned int> Offset = { 3,3 };
    unsigned int Index[] = { 0,1,2 };
    auto MixVBO = std::make_shared<CVertexBufferObject>(MixVertices, sizeof(MixVertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_COLOR_BIT, Offset);
    auto MixEBO = std::make_shared<CElementBufferObject>(Index, sizeof(Index));
    auto MixVAO = std::make_shared<CVertexArrayObject>();

    MixVAO->addVBO(MixVBO);
    MixVAO->setEBO(MixEBO);

    auto Mix = std::make_shared<CStuff>();
    Mix->addVAO(MixVAO, MixShader);

    vWindow.addStuff(Mix);

    vWindow.render();
}

static void renderMixSquare(CWindow& vWindow) {
    auto MixShader = std::make_shared<CShader>("./Shader/mix.vs", "./Shader/mix.fs");

    std::vector<unsigned int> Offset = { 3,3 };
    unsigned int Index[] = { 0,1,2, 0,2,3 };
    float MixSquareVertices[] = {
        // positions         // colors
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // top right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f    // top left 
    };
    auto MixVBO = std::make_shared<CVertexBufferObject>(MixSquareVertices, sizeof(MixSquareVertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_COLOR_BIT, Offset);
    auto MixEBO = std::make_shared<CElementBufferObject>(Index, sizeof(Index));
    auto MixVAO = std::make_shared<CVertexArrayObject>();

    MixVAO->addVBO(MixVBO);
    MixVAO->setEBO(MixEBO);

    auto Mix = std::make_shared<CStuff>();
    Mix->addVAO(MixVAO, MixShader);

    vWindow.addStuff(Mix);

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

    auto Rectangle = std::make_shared<CStuff>();
    Rectangle->addVAO(RectangleVAO, TextureShader);

    CImage Image("./assets/container.jpg");
    CTexture Texture(Image, GL_TEXTURE0);
    Texture.bind();

    vWindow.addStuff(Rectangle);

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
    CImage Container("./assets/container.jpg");
    CTexture Texture_0(Container, GL_TEXTURE0);
    CImage Awesomeface("./assets/awesomeface.png");
    CTexture Texture_1(Awesomeface, GL_TEXTURE1);

    Texture_0.bind();
    Texture_1.bind();

    std::vector<unsigned int> Offset = { 3,0,2 };
    auto TransformVBO = std::make_shared<CVertexBufferObject>(Vertices, sizeof(Vertices), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_TEXTURE_BIT, Offset);
    auto TransformEBO = std::make_shared<CElementBufferObject>(Indices, sizeof(Indices));
    auto TransformVAO = std::make_shared<CVertexArrayObject>();

    TransformVAO->addVBO(TransformVBO);
    TransformVAO->setEBO(TransformEBO);

    auto Transform = std::make_shared<CStuff>();
    Transform->addVAO(TransformVAO, TransformShader);

    CCamera Camera;
    Camera.setCameraPosition({ 0, 0, 3 });
    Camera.setFarPlane(100);
    Camera.setNearPlane(0.1);
    Camera.setAspectRatio(800.0 / 600.0);
    Camera.setFeildOfView(45.0);
    TransformShader->setProjection(Camera.getProjectionMatrix());
    TransformShader->setView(Camera.getViewMatrix());
    vWindow.addStuff(Transform);
    glEnable(GL_DEPTH_TEST);
    vWindow.render();
}

static void rotate(std::chrono::duration<double> vElapsed, CObject& vObject) {
    double Angle = M_PI * vElapsed.count() * 0.1;
    Eigen::Vector3f Axis(0, 1, 0);

    Eigen::AngleAxisf RotationVector(Angle, Axis);
    Eigen::Matrix3f RotationMatrix = RotationVector.toRotationMatrix();
    Eigen::Vector3f BeginPosition = { 0.5, 0, 0.5};
    vObject.setPosition(RotationMatrix * BeginPosition);
    //vObject.setScale(std::fmod(vElapsed.count(), 10.0) / 5.0);
}

static void scala(std::chrono::duration<double> vElapsed, CObject& vObject) {
    vObject.setScale(0.5);
}

static void renderCube(CWindow& vWindow) {
    auto TransformShader = std::make_shared<CShader>("./Shader/transform.vs", "./Shader/color.fs");
    TransformShader->use();
    TransformShader->setInt("texture1", 0);
    TransformShader->setInt("texture2", 1);
    CImage Container("./assets/container.jpg");
    CTexture Texture_0(Container, GL_TEXTURE0);
    CImage Awesomeface("./assets/awesomeface.png");
    CTexture Texture_1(Awesomeface, GL_TEXTURE1);
    Texture_0.bind();
    Texture_1.bind();

    auto LightShader = std::make_shared<CShader>("./Shader/transform.vs", "./Shader/light.fs");
    auto Cube = std::make_shared<CStuff>("./cube.txt", TransformShader);
    /*******************************/
    //WARNING: rotate is defined in main.cpp
    //Cube->setUpdateMoveFunction(rotate);
    /*******************************/
    CCamera Camera;
    Camera.setCameraPosition({ 0, 0, 3 });
    Camera.setFarPlane(100);
    Camera.setNearPlane(0.1);
    Camera.setAspectRatio(800.0 / 600.0);
    Camera.setFeildOfView(45.0);
    TransformShader->setProjection(Camera.getProjectionMatrix());
    TransformShader->setView(Camera.getViewMatrix());
    vWindow.addStuff(Cube);
    glEnable(GL_DEPTH_TEST);
    vWindow.render();
}

static void renderPhongCube(CWindow& vWindow) {

    auto PhongShader = std::make_shared<CShader>("./Shader/phong.vs", "./Shader/phong.fs");
    PhongShader->use();
    PhongShader->setInt("texture1", 0);
    PhongShader->setInt("texture2", 1);
    auto Cube = std::make_shared<CStuff>("./cube.txt", PhongShader);
    Cube->setUpdateMoveFunction(scala);

    auto LightShader = std::make_shared<CShader>("./Shader/light.vs", "./Shader/light.fs");
    auto Light = std::make_shared<CPointLight>("./light.txt", LightShader);
    Light->setUpdateMoveFunction(rotate);

    auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition({ 3, 3, 3 });
    Camera->setFarPlane(100);
    Camera->setNearPlane(0.1);
    Camera->setAspectRatio(800.0 / 600.0);
    Camera->setFeildOfView(45.0);
    PhongShader->setProjection(Camera->getProjectionMatrix());
    PhongShader->setView(Camera->getViewMatrix());
    LightShader->setProjection(Camera->getProjectionMatrix());
    LightShader->setView(Camera->getViewMatrix());
    
    CImage Container("./assets/container.jpg");
    CTexture Texture_0(Container, GL_TEXTURE0);
    Texture_0.bind();
    CImage Awesomeface("./assets/awesomeface.png");
    CTexture Texture_1(Awesomeface, GL_TEXTURE1);
    Texture_1.bind();

    vWindow.addStuff(Cube);
    vWindow.setLight(Light);
    vWindow.setCamera(Camera);
    glEnable(GL_DEPTH_TEST);
    vWindow.render();
}