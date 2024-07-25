#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <set>
#include "CObject.h"
#include "CShader.h"

/********************************/
//TODO: add CWindowConfig. inside todo just show interface.
class CWindowConfig {
public:
	int getWidth() { return 800; }
	int getHeight() { return 600; }
	int getPosX() { return 400; }
	int getPosY() { return 200; }
	std::string getTitle() { return "default title"; }
	int getMajVer() { return 4; }
	int getMinVer() { return 6; }
	bool isCore() { return true; }
};
/********************************/

class CWindow
{

public:
	CWindow();
	int initWindow(CWindowConfig vConfig);
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
	void __checkAndSetConfig(CWindowConfig vConfig);
	void __processInput();
	static void __callbackFrameBufferSize(GLFWwindow* vWindow, int vWidth, int vHeight);
};

