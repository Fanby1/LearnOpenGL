#pragma once
#include <set>
#include "def.h"
#include "CObject.h"
#include "CRenderConfig.h"
#include "CWindowConfig.h"
#include "CStuff.h"
#include "CDirectionalLight.h"
#include "CGLTFObject.h"

class GLTRAINVER3_API CWindow
{
public:
	CWindow();
	~CWindow();
	int initWindow(const CWindowConfig& vConfig);
	void startRender(const CRenderConfig& vConfig, std::function<void(std::chrono::duration<double>, CDirectionalLight&)> vFunction);
	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }

private:
	GLFWwindow* m_pWindow = nullptr;

	int m_MajVer, m_MinVer;
	int m_Width, m_Height;
	//pos: left+up
	int m_PosX, m_PosY;
	std::string m_Title;
	//m_WindowConfigIsSet only guarantee class is set, not for any variable.
	bool m_UsingCoreProfile = true, m_WindowConfigIsSet = false, m_GLMajorVerIsValid = false;
	bool m_ChangeRenderPassIsPressed = false;
	
	int m_RenderPassesNum, m_RenderPassNowAtIndex;
	std::set<std::shared_ptr<CRenderableObject>> m_RenderableObjects;
	std::vector<std::shared_ptr<CShader>> m_ShaderPrograms;
	std::shared_ptr<CGLTFObject> m_RenderStuff = nullptr;
	std::shared_ptr<CCamera> m_Camera = nullptr;
	std::shared_ptr<CPointLight> m_Light = nullptr;
	std::shared_ptr<CDirectionalLight> m_DirectionalLight = nullptr;

	void __destroyWindow();
	template<typename T>
	bool  __isParaErr(const T& vData, const T& vFloor, const T& vCeil, const std::string& vType);
	void __checkAndSetConfig(const CWindowConfig& vConfig);
	void __processInput();
	static void __callbackFrameBufferSize(GLFWwindow* vWindow, int vWidth, int vHeight);
	void __render();

	void __deleteRenderableObject(std::shared_ptr<CRenderableObject> vRenderableObject);
	void __addRenderableObject(std::shared_ptr<CRenderableObject> vRenderableObject);
	void __setCamera(std::shared_ptr<CCamera> vCamera);
	void __setDirectionalLight(std::shared_ptr<CDirectionalLight> vLight);
};

