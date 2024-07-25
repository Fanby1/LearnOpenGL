#include "CWindow.h"
#include <iostream>
#include <Windows.h>
#include "HiveLogger.h"

CWindow::CWindow() {
    int MaxWidth = GetSystemMetrics(SM_CXSCREEN);
    int MaxHeight = GetSystemMetrics(SM_CYSCREEN);
    m_Width = MaxWidth >> 1;
    m_Height = MaxHeight >> 1;
    m_PosX = MaxWidth >> 2;
    m_PosY = MaxHeight >> 2;
    m_Title = "Default Title";
    m_MajVer = 4, m_MinVer = 6;
    m_isCoreProfile = true;
    m_pWindow = nullptr;
    m_isSetPara = m_isSetMaj = false;
}
int __clampData(int vData, int vCeil, int vFloor) {
    if (vData >= vFloor && vData <= vCeil) return vData;
    if (vData < vFloor) return vFloor;
    else return vCeil;
}
bool __isParaErr(int vData, int vCeil, int vFloor,std::string vType) {
    if (__clampData(vData, vCeil, vFloor) == vData) return false;
    else {
        return true;
        HIVE_LOG_WARNING("Window gets wrong paramter {}, we will use default ones.");
    }
}

int CWindow::initWindow(CWindowConfig vConfig)
{
    int MaxWidth = GetSystemMetrics(SM_CXSCREEN);
    int MaxHeight = GetSystemMetrics(SM_CYSCREEN);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_MajVer);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_MinVer);
    glfwWindowHint(GLFW_OPENGL_PROFILE, m_isCoreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    m_pWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    if (m_pWindow == NULL)
    {
        ///**/HIVE_LOG_ERR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(m_pWindow, m_PosX, m_PosY);
    glfwMakeContextCurrent(m_pWindow);
    glfwSetFramebufferSizeCallback(m_pWindow, _framebuffer_size_callback);
    return 0;
}

void CWindow::setObject(std::set<std::shared_ptr<CObject>>&& vObjects)
{
    m_Objects = vObjects;
}

void CWindow::deleteObejct(std::shared_ptr<CObject> vObject)
{
    m_Objects.erase(vObject);
}

void CWindow::addObject(std::shared_ptr<CObject> vObject)
{
    m_Objects.insert(vObject);
}

void CWindow::render()
{
    while (!glfwWindowShouldClose(m_pWindow))
    {
        // input
        // -----
        _processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        // ..:: Drawing code (in render loop) :: ..
        for (auto& Object : m_Objects) {
            Object->render();
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
void CWindow::_processInput()
{
    if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_pWindow, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void CWindow::_framebuffer_size_callback(GLFWwindow* window, int vWidth, int vHeight)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, vWidth, vHeight);
}
