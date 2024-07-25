#include "CWindow.h"
#include <set>
#include <iostream>
#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"
#include "HiveLogger.h"

CWindow::CWindow() 
{
    m_pWindow = nullptr;
    m_MajVer = 4, m_MinVer = 6;
    m_Title = "Default Title";
    m_isCoreProfile = true;
    m_isSetPara = m_isSetMaj = false;

    int MaxWidth = GetSystemMetrics(SM_CXSCREEN);
    int MaxHeight = GetSystemMetrics(SM_CYSCREEN);
    m_Width = MaxWidth >> 1;
    m_Height = MaxHeight >> 1;
    //pos: left+up
    m_PosX = MaxWidth >> 2;
    m_PosY = MaxHeight >> 2;
    m_Objects.clear();
}

int CWindow::__clampData(const int& vData, const int& vFloor, const int& vCeil)
{
    _ASSERT(vFloor <= vCeil);
    if (vData >= vFloor && vData <= vCeil) return vData;
    if (vData < vFloor) return vFloor;
    else return vCeil;
}

bool CWindow::__isParaErr(const int& vData, const int& vFloor, const int& vCeil, const std::string& vType)
{
    if (__clampData(vData, vFloor, vCeil) == vData) return false;
    else 
    {
        return true;
        HIVE_LOG_WARNING("Window gets wrong paramter {}, we will use default parameter.", vType);
        if (vType == "OpenGL Major Version") 
            HIVE_LOG_WARNING("Because your OpenGL Major Version is wrong, we will not use the provided Minor Version.");
    }
}

void CWindow::__checkAndSetConfig(const CWindowConfig& vConfig)
{
    m_isSetPara = true;
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
    if (!__isParaErr(t, 1, 4, "OpenGL Major Version")) m_isSetMaj = true, m_MajVer = t;
    t = vConfig.getMinVer();
    if (!__isParaErr(t, 1, 6, "OpenGL Minor Version") && m_isSetMaj) m_MinVer = t;

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
        return -1;
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
    glfwMakeContextCurrent(m_pWindow);
    glfwSetWindowPos(m_pWindow, m_PosX, m_PosY);
    glfwSetFramebufferSizeCallback(m_pWindow, __callbackFrameBufferSize);
    return 0;
}

void CWindow::setStuff(std::set<std::shared_ptr<CStuff>>&& vStuffs)
{
    m_Stuffs = vStuffs;
}

void CWindow::deleteStuff(std::shared_ptr<CStuff> vStuff)
{
    m_Stuffs.erase(vStuff);
}

void CWindow::addStuff(std::shared_ptr<CStuff> vStuff)
{
    m_Stuffs.insert(vStuff);
}

void CWindow::setCamera(std::shared_ptr<CCamera> vCamera)
{
    m_Camera = vCamera;
}

void CWindow::setLight(std::shared_ptr<CLight> vLight)
{
    m_Light = vLight;
}

void CWindow::render()
{
    if (!m_isSetPara) 
    {
        HIVE_LOG_WARNING("No Config is loaded! We will use default parameters...");
    }
    while (!glfwWindowShouldClose(m_pWindow))
    {
        // input
        // -----
        __processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        // ..:: Drawing code (in render loop) :: ..
        m_Light->renderV(m_Camera, m_Light);
        for (auto& Stuff : m_Stuffs) {
            Stuff->renderV(m_Camera, m_Light);
        }
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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
