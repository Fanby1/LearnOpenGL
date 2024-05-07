#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
class CShaderFrame
{
private:
public:
	static unsigned int generateShader(const char* vSource, unsigned int Type);
};

