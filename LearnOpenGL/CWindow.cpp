#include "CWindow.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"
#include "CObject.h"
#include "CTexture.h"
#include "CVertexArrayObject.h"
#include "CVertexBufferObject.h"
#include "CElementBufferObject.h"
#include "CCamera.h"
#include "HiveLogger.h"
#include "CRenderConfig.h"

CWindow::CWindow() 
{
    m_pWindow = nullptr;
    m_MajVer = 4, m_MinVer = 6;
    m_Title = "Default Title";
    m_isCoreProfile = true;
    m_isWindowParasSet = m_isMajorVersionValid = false;

    int MaxWidth = GetSystemMetrics(SM_CXSCREEN);
    int MaxHeight = GetSystemMetrics(SM_CYSCREEN);
    m_Width = MaxWidth >> 1;
    m_Height = MaxHeight >> 1;
    //pos: left+up
    m_PosX = MaxWidth >> 2;
    m_PosY = MaxHeight >> 2;
    m_Stuffs.clear();
}

CWindow::~CWindow()
{
    __destroyWindow();
}

template<typename T>
bool CWindow::__isParaErr(const T& vData, const T& vFloor, const T& vCeil, const std::string& vType)
{
    if (std::clamp(vData, vFloor, vCeil) == vData) return false;
    else 
    {
        HIVE_LOG_WARNING("Window gets wrong paramter {}, we will use default parameter.", vType);
        if (vType == "OpenGL Major Version") 
            HIVE_LOG_WARNING("Because your OpenGL Major Version is wrong, we will not use the provided Minor Version.");
        return true;
    }
}

void CWindow::__destroyWindow()
{
    if (m_pWindow != nullptr) {
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }
}

void CWindow::__checkAndSetConfig(const CWindowConfig& vConfig)
{
    m_isWindowParasSet = true;
    std::string TempTitle = vConfig.getTitle();
    if (TempTitle.empty()) 
    {
        HIVE_LOG_WARNING("Empty title provided! We will use default title.");
    }
    else 
    {
        m_Title = TempTitle;
    }
    int t = 0;
    t = vConfig.getMajVer();
    if (!__isParaErr(t, 1, 4, "OpenGL Major Version")) m_isMajorVersionValid = true, m_MajVer = t;
    t = vConfig.getMinVer();
    if (!__isParaErr(t, 1, 6, "OpenGL Minor Version") && m_isMajorVersionValid) m_MinVer = t;

    int MaxWidth = GetSystemMetrics(SM_CXSCREEN);
    int MaxHeight = GetSystemMetrics(SM_CYSCREEN);
    t = vConfig.getWidth();
    if (!__isParaErr(t, 0, MaxWidth, "Width")) m_Width = t;
    t = vConfig.getHeight();
    if (!__isParaErr(t, 0, MaxHeight, "Height")) m_Height = t;
    t = vConfig.getPosX();
    if (!__isParaErr(t, 0, MaxWidth - m_Width, "PosX")) m_PosX = t;
    t = vConfig.getPosY();
    if (!__isParaErr(t, 0, MaxHeight - m_Height, "PosY")) m_PosY = t;
    m_isCoreProfile = vConfig.isCore();
}

int CWindow::initWindow(const CWindowConfig& vConfig)
{
    if (!vConfig.isInit()) 
    {
        HIVE_LOG_WARNING("Config Is not initialized! We will use default value.");
    }
    
    glfwInit();
    if (m_pWindow != nullptr) {
        HIVE_LOG_WARNING("Destroied existing window! Please check whether multiplied created window in main.cpp.");
        __destroyWindow();
    }
    __checkAndSetConfig(vConfig);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_MajVer);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_MinVer);
    glfwWindowHint(GLFW_OPENGL_PROFILE, m_isCoreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    
    m_pWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    if (m_pWindow == NULL)
    {
        HIVE_LOG_ERROR("Failed to create GLFW window!");
        glfwTerminate();
        return -1;
    }
    HIVE_LOG_INFO("Create GLFW Window successfully.");
    
    glfwMakeContextCurrent(m_pWindow);
    glfwSetWindowPos(m_pWindow, m_PosX, m_PosY);
    glfwSetFramebufferSizeCallback(m_pWindow, __callbackFrameBufferSize);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        HIVE_LOG_ERROR("Failed to initialize GLAD");
    }
    return 0;
}

void CWindow::startRender(const CRenderConfig& vConfig, std::function<void(std::chrono::duration<double>, CDirectionalLight&)> vFunction)
{
    std::string VSPath = vConfig.isInit(0) ? vConfig.getVertexShaderPath(0) : "./Shader/directionalLight.vs";
    std::string FSPath = vConfig.isInit(0) ? vConfig.getFragmentShaderPath(0) : "./Shader/directionalLight.fs";
    CRenderConfig::ERenderPassType UsePerVertexShading = vConfig.getRenderPassType(0);
    HIVE_LOG_INFO("Using render pass: {}", UsePerVertexShading == CRenderConfig::ERenderPassType::UES_PER_VERTEX_SHADING ? "per vertex shading" : "per pixel shading");

    auto DirectialLightShader = std::make_shared<CShader>(VSPath.c_str(), FSPath.c_str());
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

    auto Cube = std::make_shared<CStuff>("./assets/cube.txt", DirectialLightShader);
    // Cube->setUpdateMoveFunction(scala);
    auto DirectionalLight = std::make_shared<CDirectionalLight>();
    DirectionalLight->setUpdateMoveFunction(vFunction);

    auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition({ 2, 2, 2 });
    Camera->setFarPlane(100);
    Camera->setNearPlane(0.1);
    Camera->setAspectRatio(1.0 * m_Width / m_Height);
    Camera->setFeildOfView(45.0);
    __addStuff(Cube);
    __setDirectionalLight(DirectionalLight);
    __setCamera(Camera);
    glEnable(GL_DEPTH_TEST);

    __render();
}

void CWindow::__render()
{
    if (!m_isWindowParasSet)
    {
        HIVE_LOG_WARNING("No Window Config is loaded! We will use default parameters...");
    }

    while (!glfwWindowShouldClose(m_pWindow))
    {
        __processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_Light) {
            m_Light->renderV(m_Camera, m_Light, m_DirectionalLight);
        }
        for (auto& Stuff : m_Stuffs) {
            Stuff->renderV(m_Camera, m_Light, m_DirectionalLight);
        }
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
    glfwTerminate();
}

void CWindow::__processInput()
{
    if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_pWindow, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void CWindow::__callbackFrameBufferSize(GLFWwindow* window, int vWidth, int vHeight)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, vWidth, vHeight);
}

//never used
//void CWindow::setStuff(std::set<std::shared_ptr<CStuff>>&& vStuffs)
//{
//    m_Stuffs = vStuffs;
//}

//void CWindow::deleteStuff(std::shared_ptr<CStuff> vStuff)
//{
//    m_Stuffs.erase(vStuff);
//}

void CWindow::__addStuff(std::shared_ptr<CStuff> vStuff)
{
    m_Stuffs.insert(vStuff);
}

void CWindow::__setCamera(std::shared_ptr<CCamera> vCamera)
{
    m_Camera = vCamera;
}

//void CWindow::setLight(std::shared_ptr<CPointLight> vLight)
//{
//    m_Light = vLight;
//}

void CWindow::__setDirectionalLight(std::shared_ptr<CDirectionalLight> vLight)
{
    m_DirectionalLight = vLight;
}
