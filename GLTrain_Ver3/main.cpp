#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include "HiveLogger.h"
#include "CWindow.h"
#include "CRenderConfig.h"
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
    double Angle = M_PI * 3e-4;
    Eigen::Vector3f Axis(0, 1, 1);//normalized during process
    vLight.rotate(Angle, Axis);
}
/*
int main() {
    CWindowConfig WConfig("./assets/WConfig.xml");
    WConfig.init();

    CWindow GLFWWindow;
    GLFWWindow.initWindow(WConfig);

    //CRenderConfig RConfig("./assets/RConfig_vertex.xml");
    CRenderConfig RConfig("./assets/RConfig_switch.xml");
    RConfig.init();
    initGLAD();
    GLFWWindow.startRender(RConfig, roateByY);
    return 0;
}
*/
int main() {  
    CWindowConfig WConfig("./assets/WConfig.xml");
    WConfig.init();

    CWindow GLFWWindow;
    GLFWWindow.initWindow(WConfig);
    CGLTFObject Obeject;
    Obeject.loadModelFromGLTF("./assets/dragon.gltf");
    CRenderConfig RConfig("./assets/RConfig_switch.xml");
    RConfig.init();
    GLFWWindow.startRender(RConfig, roateByY);
}
