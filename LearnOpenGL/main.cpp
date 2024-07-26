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

void renderDirectialLight(CWindow& vWindow,const CRenderConfig& vRConfig) 
{
    initGLAD();
    std::string VSPath = vRConfig.isInit() ? vRConfig.getVertexShaderPath() : "./Shader/directionalLight.vs";
    std::string FSPath = vRConfig.isInit() ? vRConfig.getFragmentShaderPath() : "./Shader/directionalLight.fs";
    bool UsePerVertexShading = vRConfig.isInit() ? vRConfig.isUsingPerVertexShading() : false;

    vWindow.render(VSPath,FSPath, roateByY);
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