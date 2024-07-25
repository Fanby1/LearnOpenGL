#pragma once
#include "CObject.h"
#include <set>
#include "CWindowConfig.h"

class CWindow
{

public:
	CWindow();
	int initWindow(CWindowConfig vConfig);
	void setStuff(std::set<std::shared_ptr<CStuff>>&& vStuffs);
	void deleteStuff(std::shared_ptr<CStuff> vStuff);
	void addStuff(std::shared_ptr<CStuff> vStuff);
	void setCamera(std::shared_ptr<CCamera> vCamera);
	void setLight(std::shared_ptr<CLight> vLight);
	void render();

private:
	GLFWwindow* m_pWindow = nullptr;

	int m_MajVer, m_MinVer;
	int m_Width, m_Height;
	//pos: left+up
	int m_PosX, m_PosY;
	std::string m_Title;
	//m_isSetPara only guarantee class is set, not for any variable.
	bool m_isCoreProfile, m_isSetPara, m_isSetMaj;
	
	std::set<std::shared_ptr<CStuff>> m_Stuffs;
	std::shared_ptr<CCamera> m_Camera;
	std::shared_ptr<CLight> m_Light;

	int __clampData(const int& vData, const int& vFloor, const int& vCeil);
	bool  __isParaErr(const int& vData, const int& vFloor, const int& vCeil, const std::string& vType);
	void __checkAndSetConfig(const CWindowConfig& vConfig);
	void __processInput();
	static void __callbackFrameBufferSize(GLFWwindow* vWindow, int vWidth, int vHeight);
};

