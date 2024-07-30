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
#include <iostream>

// º¯Êý¼ì²é OpenGL ´íÎó
void checkOpenGLError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    while (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << err << " at " << stmt << " in " << fname << " at line " << line << std::endl;
        err = glGetError();
    }
}

#define CHECK_GL_ERROR(stmt) do { \
    stmt; \
    checkOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)

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
    
    auto FrameBuffer = std::make_shared<CFramebuffer>(GLFWWindow.getWidth(), GLFWWindow.getHeight());

    FrameBuffer->createAndAddGBuffer("gPosition", GL_TEXTURE2, GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT);
    FrameBuffer->createAndAddGBuffer("gNormal", GL_TEXTURE3, GL_COLOR_ATTACHMENT1, GL_RGB16F, GL_RGB, GL_FLOAT);
    FrameBuffer->createAndAddGBuffer("gAlbedoSpec", GL_TEXTURE4, GL_COLOR_ATTACHMENT2, GL_RGBA, GL_RGBA, GL_FLOAT);
    FrameBuffer->createAndAddDepthBuffer("gDepthTexture", GL_TEXTURE5, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    FrameBuffer->render();
    
    
    auto gShader = std::make_shared<CShader>("./Shader/geometry_pass.vs", "./Shader/geometry_pass.fs");
    auto lShader = std::make_shared<CShader>("./Shader/lighting_pass.vs", "./Shader/lighting_pass.fs");

    HIVE_LOG_INFO("gShader ID: {}", gShader->getID());
    HIVE_LOG_INFO("lShader ID: {}", lShader->getID());

    gShader->use();
    gShader->setInt("material.diffuse", 0);
    gShader->setInt("material.specular", 1);
    gShader->setFloat("material.shininess", 32);

    lShader->use();
    lShader->setFloat("material.shininess", 32);
    
    
    auto Dragon = std::make_shared<CGLTFObject>("./assets/Dragon.gltf");
    
    for(const auto& VAO : Dragon->getVAOs())
	{
		Dragon->setVAOGeometryShader(VAO, gShader);
		Dragon->setVAOLightingShader(VAO, lShader);
	}
    
    GLFWWindow.addRenderableObject(Dragon);

    CRenderConfig RConfig("./assets/RConfig_switch.xml");
    RConfig.initV();
    
    auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition({ 0, -20, -20 });
    Camera->setFarPlane(100);
    Camera->setNearPlane(0.1);
    Camera->setAspectRatio(1.0 * GLFWWindow.getWidth() / GLFWWindow.getHeight());
    Camera->setFeildOfView(45.0);
    GLFWWindow.setCamera(Camera);
    /*
    *     auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition({ 0, -20, -20 });
    Camera->setFarPlane(100);
    Camera->setNearPlane(0.1);
    Camera->setAspectRatio(1.0 * GLFWWindow.getWidth() / GLFWWindow.getHeight());
    Camera->setFeildOfView(45.0);
    GLFWWindow.setCamera(Camera);
    */

    auto DirectionalLight = std::make_shared<CDirectionalLight>();
    DirectionalLight->setUpdateMoveFunction(roateByY);
    GLFWWindow.setDirectionalLight(DirectionalLight);

    GLFWWindow.setFrameBuffer(FrameBuffer);

    GLFWWindow.renderDeferred();
}

