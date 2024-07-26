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
#include "CRenderConfig.h"

//TODO: write glfwINIT() in dllmain.cpp

static void initGLAD() 
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        HIVE_LOG_ERROR("Failed to initialize GLAD");
    }
}

static void roateByY(std::chrono::duration<double> vElapsed, CDirectionalLight& vLight) 
{
    double Angle = M_PI * vElapsed.count() * 0.001;
	Eigen::Vector3f Axis(0, 1, 1);
    vLight.rotate(Angle, Axis);
}

static void renderDirectialLight(CWindow& vWindow,CRenderConfig& vRConfig) 
{
    initGLAD();
	auto DirectialLightShader = std::make_shared<CShader>(vRConfig.getVertexShaderPath().c_str(), vRConfig.getFragmentShaderPath().c_str());
    DirectialLightShader->use();
    DirectialLightShader->setInt("material.diffuse", 0);
    DirectialLightShader->setInt("material.specular", 1);
    CImage Container("./assets/container2.png");
    CTexture Texture_0(Container, GL_TEXTURE0);
    Texture_0.bind();
    CImage Specular("./assets/container2_specular.png");
    CTexture Texture_1(Specular, GL_TEXTURE1);
    Texture_1.bind();
    DirectialLightShader->setFloat("material.shininess", 64.0f);

    auto Cube = std::make_shared<CStuff>("./cube.txt", DirectialLightShader);
    // Cube->setUpdateMoveFunction(scala);
    auto DirectionalLight = std::make_shared<CDirectionalLight>();
    DirectionalLight->setUpdateMoveFunction(roateByY);

    auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition({ 0, 0, 3 });
    Camera->setFarPlane(100);
    Camera->setNearPlane(0.1);
    Camera->setAspectRatio(800.0 / 600.0);
    Camera->setFeildOfView(45.0);
    vWindow.addStuff(Cube);
    vWindow.setDirectionalLight(DirectionalLight);
    vWindow.setCamera(Camera);
    glEnable(GL_DEPTH_TEST);
    vWindow.render();
}

int main() {
    CWindowConfig WConfig("./assets/WConfig.xml");
    WConfig.init();

    CWindow Window;
    Window.initWindow(WConfig);

    CRenderConfig RConfig("./assets/RConfig.xml");
    RConfig.init();

    renderDirectialLight(Window, RConfig);
}