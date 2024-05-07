#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class CWindow
{
private:
	GLFWwindow* m_Window;
	int m_Width, m_Height;
	void _processInput();
	static void _framebuffer_size_callback(GLFWwindow* vWindow, int vWidth, int vHeight);
public:
	CWindow(unsigned int vWidth, unsigned int vHeight);
	void render(std::vector<unsigned int> vShaderProgram, std::vector<unsigned int> vVAO);
};

