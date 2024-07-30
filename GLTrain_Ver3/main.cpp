#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include "HiveLogger.h"
#include "CWindow.h"
#include "CObjectConfig.h"
#include "CRenderConfig.h"
#include "CShader.h"
#include "CGLTFObject.h"
#include "CCamera.h"
#include "CGBuffer.h"
#include "CFramebuffer.h"
#include <iostream>

static void roateByY(std::chrono::duration<double> vElapsed, CDirectionalLight& vLight)
{
    double Angle = M_PI * 1e-3;
    Eigen::Vector3f Axis(0, 1, -1);//normalized during process
    vLight.rotate(Angle, Axis);
}

int main() {
    CWindowConfig WConfig("./assets/WConfig.xml");
    WConfig.initV();

    CWindow GLFWWindow;
    GLFWWindow.initWindow(WConfig);

    CRenderConfig RConfig("./assets/RConfig_switch.xml");
    RConfig.initV();
    CObjectConfig OConfig("./assets/FConfig.xml");
    OConfig.initV();
    GLFWWindow.initRenderPara(RConfig, OConfig, roateByY);
}

