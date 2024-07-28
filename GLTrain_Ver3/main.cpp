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
    CShader gShader("./Shader/geometry_pass.vs", "./Shader/geometry_pass.fs");
    CShader lShader("./Shader/lighting_pass.vs", "./Shader/lighting_pass.fs");

    CGLTFObject Dragon("./assets/Dragon.gltf", gShader);
}
