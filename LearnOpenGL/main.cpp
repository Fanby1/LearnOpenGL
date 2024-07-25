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

//TODO: write glfwINIT() in dllmain.cpp

static void initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        HIVE_LOG_ERROR("Failed to initialize GLAD");
    }
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

static void renderPhongCube(CWindow& vWindow) {

    auto PhongShader = std::make_shared<CShader>("./Shader/phong.vs", "./Shader/phong.fs");
    PhongShader->use();
    PhongShader->setInt("texture1", 0);
    PhongShader->setInt("texture2", 1);
    auto Cube = std::make_shared<CStuff>("./cube.txt", PhongShader);
    Cube->setUpdateMoveFunction(rotate);

    auto LightShader = std::make_shared<CShader>("./Shader/light.vs", "./Shader/light.fs");
    auto Light = std::make_shared<CLight>("./light.txt", LightShader);
    Light->setUpdateMoveFunction(rotate);

    auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition({ 0, 0, 3 });
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

int main() {
    CWindowConfig AConfig("./assets/Config.xml");
    AConfig.init();
    CWindow Window;
    Window.initWindow(AConfig);
    initGLAD();

    renderPhongCube(Window);
}