#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include "HiveLogger.h"
#include "CWindow.h"
#include "CRenderConfig.h"
#include "CShader.h"
#include "CGLTFObject.h"
#include "CCamera.h"
#include "CGBuffer.h"
#include "CFramebuffer.h"

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
    double Angle = M_PI * 1e-3;
    Eigen::Vector3f Axis(0, 1, -1);//normalized during process
    vLight.rotate(Angle, Axis);
}
/*
int main() {  
    CWindowConfig WConfig("./assets/WConfig.xml");
    WConfig.init();

    CWindow GLFWWindow;
    GLFWWindow.initWindow(WConfig);
    CRenderConfig RConfig("./assets/RConfig_switch.xml");
    RConfig.init();
    GLFWWindow.startRender(RConfig, roateByY);
}
*/


int main() {
    CWindowConfig WConfig("./assets/WConfig.xml");
    WConfig.init();

    CWindow GLFWWindow;
    GLFWWindow.initWindow(WConfig);

    auto FrameBuffer = std::make_shared<CFramebuffer>(GLFWWindow.getWidth(), GLFWWindow.getHeight());

    FrameBuffer->createAndAddGBuffer(2, GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT);
    FrameBuffer->createAndAddGBuffer(3, GL_COLOR_ATTACHMENT1, GL_RGB16F, GL_RGB, GL_FLOAT);
    FrameBuffer->createAndAddGBuffer(4, GL_COLOR_ATTACHMENT2, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    auto gShader = std::make_shared<CShader>("./Shader/geometry_pass.vs", "./Shader/geometry_pass.fs");
    auto lShader = std::make_shared<CShader>("./Shader/lighting_pass.vs", "./Shader/lighting_pass.fs");

    gShader->setInt("material.diffuse", 0);
    gShader->setInt("material.specular", 1);
    gShader->setFloat("material.shininess", 32);

    auto Dragon = std::make_shared<CGLTFObject>("./assets/Dragon.gltf");
    for(const auto& VAO : Dragon->getVAOs())
	{
		Dragon->setVAOGeometryShader(VAO, gShader);
		Dragon->setVAOLightingShader(VAO, gShader);
	}
    CRenderConfig RConfig("./assets/RConfig_switch.xml");
    RConfig.init();

    auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition({ 0, -20, -20 });
    Camera->setFarPlane(100);
    Camera->setNearPlane(0.1);
    Camera->setAspectRatio(1.0 * GLFWWindow.getWidth() / GLFWWindow.getHeight());
    Camera->setFeildOfView(45.0);
    GLFWWindow.setCamera(Camera);

    GLFWWindow.addRenderableObject(Dragon);

    auto DirectionalLight = std::make_shared<CDirectionalLight>();
    DirectionalLight->setUpdateMoveFunction(roateByY);
    GLFWWindow.setDirectionalLight(DirectionalLight);

    GLFWWindow.setFrameBuffer(FrameBuffer);

    GLFWWindow.renderDeferred();
}

