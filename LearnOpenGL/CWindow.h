#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <set>
#include "CObject.h"
#include "CShader.h"

class CWindow
{
private:
	GLFWwindow* m_Window;
	int m_Width, m_Height;
	std::set<std::shared_ptr<CObject>> m_Objects;
	void _processInput();
	static void _framebuffer_size_callback(GLFWwindow* vWindow, int vWidth, int vHeight);
public:
	CWindow(unsigned int vWidth, unsigned int vHeight);
	void setObject(std::set<std::shared_ptr<CObject>>&& vObjects);
	void deleteObejct(std::shared_ptr<CObject> vObject);
	void addObject(std::shared_ptr<CObject> vObject);
	void render();
};

