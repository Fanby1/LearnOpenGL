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
#include "CGLTFObject.h"
#include <iostream>

// 函数检查 OpenGL 错误
void checkOpenGLError(const char* vState, const char* vFile, int vLineNum) {
    GLenum err = glGetError();
    while (err != GL_NO_ERROR) {
        HIVE_LOG_ERROR("OpenGL ERR: {} at {} in {} at line {}.", err, vState, vFile, vLineNum);
        err = glGetError();
    }
}

#define CHECK_GL_ERROR(stmt) do { \
    stmt; \
    checkOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)

CWindow::CWindow() 
{
    m_pWindow = nullptr;
    m_MajVer = 4, m_MinVer = 6;
    m_Title = "Default Title";
    m_UsingCoreProfile = true;
    m_WindowConfigIsSet = m_GLMajorVerIsValid = false;

    int MaxWidth = GetSystemMetrics(SM_CXSCREEN);
    int MaxHeight = GetSystemMetrics(SM_CYSCREEN);
    m_Width = MaxWidth >> 1;
    m_Height = MaxHeight >> 1;
    //pos: left+up
    m_PosX = MaxWidth >> 2;
    m_PosY = MaxHeight >> 2;
    m_RenderableObjects.clear();
    m_ShaderPrograms.clear();
    m_RenderPassesNum = -1;
    m_RenderPassNowAtIndex = 0;
    m_ChangeRenderPassIsPressed = false;
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
        {
            HIVE_LOG_WARNING("Because your OpenGL Major Version is wrong, we will not use the provided Minor Version.");
        }
        return true;
    }
}

void CWindow::__destroyWindow()
{
    if (m_pWindow != nullptr) 
    {
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }
}

void CWindow::__checkAndSetConfig(const CWindowConfig& vConfig)
{
    m_WindowConfigIsSet = true;
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
    if (!__isParaErr(t, 1, 4, "OpenGL Major Version")) m_GLMajorVerIsValid = true, m_MajVer = t;
    t = vConfig.getMinVer();
    if (!__isParaErr(t, 1, 6, "OpenGL Minor Version") && m_GLMajorVerIsValid) m_MinVer = t;

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
    m_UsingCoreProfile = vConfig.isCore();
}

int CWindow::initWindow(const CWindowConfig& vConfig)
{
    if (!vConfig.isInit()) 
    {
        HIVE_LOG_WARNING("Config Is not initialized! We will use default value.");
    }
    
    glfwInit();
    if (m_pWindow != nullptr) 
    {
        HIVE_LOG_WARNING("Destroied existing window! Please check whether multiplied created window in main.cpp.");
        __destroyWindow();
    }
    __checkAndSetConfig(vConfig);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_MajVer);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_MinVer);
    glfwWindowHint(GLFW_OPENGL_PROFILE, m_UsingCoreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    
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

void CWindow::initRenderPara(const CRenderConfig& vConfig, const CObjectConfig& vObjectConfig, std::function<void(std::chrono::duration<double>, CObject&)> vFunction)
{
    if (!m_WindowConfigIsSet)
    {
        HIVE_LOG_WARNING("No Window Config is loaded! We will use default parameters...");
    }
    m_RenderPassesNum = vConfig.getRenderPassNum();
    m_RenderPassNowAtIndex = 0;
    for (int i = 0; i < m_RenderPassesNum; ++i)
    {
        bool isRConfigValid = vConfig.isInit(i);
        if (!isRConfigValid) 
        {
            HIVE_LOG_ERROR("Window do not load Config {} correctly.", i);
            continue;
        }
        std::string VSPath = vConfig.getVertexShaderPath(i);
        std::string FSPath = vConfig.getFragmentShaderPath(i);
        CRenderConfig::ERenderPassType UsePerVertexShading = vConfig.getRenderPassType(i);
        auto ShaderProgram = std::make_shared<CShader>(VSPath.c_str(), FSPath.c_str());
        ShaderProgram->use();
        for (const auto& Uniform : vConfig.getUniformsInPass(i))
        {
            std::string TolowerUniformType = Uniform._UType;
            transform(TolowerUniformType.begin(), TolowerUniformType.end(), TolowerUniformType.begin(), ::tolower);
            //HIVE_LOG_INFO("{} : {} and {}", Uniforms._UName, Uniforms._UType, atoi(Uniforms._UValue.c_str()));
            if (TolowerUniformType == "int") ShaderProgram->setInt(Uniform._UName, atoi(Uniform._UValue.c_str()));
            if (TolowerUniformType == "float") ShaderProgram->setFloat(Uniform._UName, atof(Uniform._UValue.c_str()));
        }
        m_ShaderPrograms.push_back(ShaderProgram);
    }

    auto GeoShader = m_ShaderPrograms[0];
    auto LightShader = m_ShaderPrograms[1];

    HIVE_LOG_INFO("gShader ID: {}", GeoShader->getID());
    HIVE_LOG_INFO("lShader ID: {}", LightShader->getID());

    auto Dragon = std::make_shared<CGLTFObject>(vObjectConfig.getModelPath());
    for (const auto& VAO : Dragon->getVAOs())
    {
        Dragon->setVAOGeometryShader(VAO, GeoShader);
        Dragon->setVAOLightingShader(VAO, LightShader);
    }
    addRenderableObject(Dragon);

    auto Cube = std::make_shared<CStuff>("./assets/light.txt");
    Cube->move({ 0.0f, - 10.f, - 15.f });
    for (const auto& VAO : Cube->getVAOs())
    {
        Cube->setVAOGeometryShader(VAO, GeoShader);
        Cube->setVAOLightingShader(VAO, LightShader);
    }
    addRenderableObject(Cube);

    auto Camera = std::make_shared<CCamera>();
    Camera->setCameraPosition(vObjectConfig.getCameraPos());
    Camera->setFarPlane(vObjectConfig.getFarPlane());
    Camera->setNearPlane(vObjectConfig.getNearPlane());
    Camera->setAspectRatio(1.0 * getWidth() / getHeight());
    Camera->setFeildOfView(vObjectConfig.getViewField());
    setCamera(Camera);

    auto DirectionalLight = std::make_shared<CDirectionalLight>();
    DirectionalLight->setUpdateMoveFunction(vFunction);
    DirectionalLight->setNameInShader("light");
    DirectionalLight->setColor({ 1.0f, 0.f, 0.f });
    setDirectionalLight(DirectionalLight);

    auto PointLight = std::make_shared<CPointLight>();
    PointLight->setColor({ 1.0f, 0.0f, 1.0f });
    PointLight->move({ 0.0f, -19.0f, -19.0f });
    PointLight->setNameInShader("pointLight");
    setPointLight(PointLight);

    auto FrameBuffer = std::make_shared<CFramebuffer>(getWidth(), getHeight());

    FrameBuffer->createAndAddGBuffer("gPosition", GL_TEXTURE2, GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT);
    FrameBuffer->createAndAddGBuffer("gNormal", GL_TEXTURE3, GL_COLOR_ATTACHMENT1, GL_RGB16F, GL_RGB, GL_FLOAT);
    FrameBuffer->createAndAddGBuffer("gAlbedoSpec", GL_TEXTURE4, GL_COLOR_ATTACHMENT2, GL_RGBA, GL_RGBA, GL_FLOAT);
    FrameBuffer->createAndAddDepthBuffer("gDepthTexture", GL_TEXTURE5, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    FrameBuffer->render();
    setFrameBuffer(FrameBuffer);
    renderDeferred();
}

void CWindow::renderPixel()
{
    while (!glfwWindowShouldClose(m_pWindow))
    {
        glfwPollEvents();
        __processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& RenderableObject : m_RenderableObjects) 
        {
            RenderableObject->renderV(m_Camera, m_Light, m_DirectionalLight);
        }
        glfwSwapBuffers(m_pWindow);
    }
    glfwTerminate();
}

void CWindow::renderDeferred()
{
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(m_pWindow)) // 游戏循环
    {
        glfwPollEvents();
        __processInput();
        // 1. 几何处理阶段：渲染所有的几何/颜色数据到G缓冲 
        m_FramBuffer->bind();
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_GL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        for (auto& RenderableObject : m_RenderableObjects)
        {
            RenderableObject->renderGeometryV(m_Camera);
        }
        
        // 2. 光照处理阶段：使用G缓冲计算场景的光照
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        __renderGrad(m_Camera, m_Light, m_DirectionalLight);
        glfwSwapBuffers(m_pWindow);
    }
}

void CWindow::__processInput()
{
    if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_pWindow, true);
    }

    if (glfwGetKey(m_pWindow, m_KeyToSwitchRenderPass) == GLFW_PRESS && !m_ChangeRenderPassIsPressed)
    {
      /*  m_RenderPassNowAtIndex = (m_RenderPassNowAtIndex + 1) % m_RenderPassesNum;
        HIVE_LOG_INFO("Switch to render pass number: {}", m_RenderPassNowAtIndex);
        (*m_RenderableObjects.begin())->setVAOForwardShader((*m_RenderableObjects.begin())->getVAOs()[0], m_ShaderPrograms[m_RenderPassNowAtIndex]);*/

        m_ChangeRenderPassIsPressed = true;
    }
    if (glfwGetKey(m_pWindow, m_KeyToSwitchRenderPass) == GLFW_RELEASE)
    {
        m_ChangeRenderPassIsPressed = false;
    }
}

void CWindow::__renderGrad(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight)
{
    std::shared_ptr<CVertexArrayObject> VAO = (*m_RenderableObjects.begin())->getVAOs()[0];
    const auto& LightShader = (*m_RenderableObjects.begin())->getVAOShaders(VAO).m_LightingShader;
    LightShader->use();
    const auto& GBuffers = m_FramBuffer->getGBuffers();
    for (const auto& GBuffer : GBuffers)
	{
		GBuffer->bind();
        GBuffer->setUniformTexture(LightShader);
	}
    m_FramBuffer->getDepthBuffer()->bind();
    m_FramBuffer->getDepthBuffer()->setUniformTexture(LightShader);
    vCamera->updateShaderUniforms(LightShader);
    if (vLight) 
    {
        vLight->updateShaderUniforms(LightShader);
    }
    if (vDirectionalLight) 
	{
		vDirectionalLight->updateShaderUniforms(LightShader);
	}
    VAO->bind();
    CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void CWindow::__callbackFrameBufferSize(GLFWwindow* window, int vWidth, int vHeight)
{
    glViewport(0, 0, vWidth, vHeight);
}

void CWindow::deleteRenderableObject(std::shared_ptr<CRenderableObject> vRenderableObject)
{
    m_RenderableObjects.erase(vRenderableObject);
}

void CWindow::addRenderableObject(std::shared_ptr<CRenderableObject> vRenderableObject)
{
    m_RenderableObjects.insert(vRenderableObject);
}

void CWindow::setCamera(std::shared_ptr<CCamera> vCamera)
{
    m_Camera = vCamera;
}

void CWindow::setDirectionalLight(std::shared_ptr<CDirectionalLight> vLight)
{
    m_DirectionalLight = vLight;
}

void CWindow::setPointLight(std::shared_ptr<CPointLight> vLight)
{
    m_Light = vLight;
}
