#pragma once
#include <set>
#include "CObject.h"
#include "CWindowConfig.h"
#include "CStuff.h"
#include "CDirectionalLight.h"

class CWindow
{

public:
	CWindow();
	~CWindow();
	int initWindow(const CWindowConfig& vConfig);
	void render();
	const int getWidth() { return m_Width; }
	const int getHeight() { return m_Height; }

	//neverused void setStuff(std::set<std::shared_ptr<CStuff>>&& vStuffs);
	void deleteStuff(std::shared_ptr<CStuff> vStuff);
	void addStuff(std::shared_ptr<CStuff> vStuff);
	void setCamera(std::shared_ptr<CCamera> vCamera);
	void setLight(std::shared_ptr<CPointLight> vLight);
	void setDirectionalLight(std::shared_ptr<CDirectionalLight> vLight);

private:
	GLFWwindow* m_pWindow = nullptr;

	int m_MajVer, m_MinVer;
	int m_Width, m_Height;
	//pos: left+up
	int m_PosX, m_PosY;
	std::string m_Title;
	//m_isSetPara only guarantee class is set, not for any variable.
	bool m_isCoreProfile, m_isParasSet, m_isMajorVersionValid;
	
	std::set<std::shared_ptr<CStuff>> m_Stuffs;
	std::shared_ptr<CCamera> m_Camera = nullptr;
	std::shared_ptr<CPointLight> m_Light = nullptr;
	std::shared_ptr<CDirectionalLight> m_DirectionalLight = nullptr;

	void __destroyWindow();
	template<typename T>
	bool  __isParaErr(const T& vData, const T& vFloor, const T& vCeil, const std::string& vType);
	void __checkAndSetConfig(const CWindowConfig& vConfig);
	void __processInput();
	static void __callbackFrameBufferSize(GLFWwindow* vWindow, int vWidth, int vHeight);
};

