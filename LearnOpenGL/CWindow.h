#pragma once
#include "CObject.h"
#include <set>
#include "CWindowConfig.h"

class CWindow
{

public:
	CWindow();
	int initWindow(const CWindowConfig& vConfig);
	void setObject(std::set<std::shared_ptr<CObject>>&& vObjects);
	void deleteObejct(std::shared_ptr<CObject> vObject);
	void addObject(std::shared_ptr<CObject> vObject);
	void render();

private:
	GLFWwindow* m_pWindow;

	int m_MajVer, m_MinVer;
	int m_Width, m_Height;
	//pos: left+up
	int m_PosX, m_PosY;
	std::string m_Title;
	//m_isSetPara only guarantee class is set, not for any variable.
	bool m_isCoreProfile, m_isSetPara, m_isSetMaj;
	
	std::set<std::shared_ptr<CObject>> m_Objects;
	int __clampData(const int& vData, const int& vFloor, const int& vCeil);
	bool  __isParaErr(const int& vData, const int& vFloor, const int& vCeil, const std::string& vType);
	void __checkAndSetConfig(const CWindowConfig& vConfig);
	void __processInput();
	static void __callbackFrameBufferSize(GLFWwindow* vWindow, int vWidth, int vHeight);
};

