#pragma once
#include <stb_image.h>
#include <tiny_gltf.h>
#include <GLFW/glfw3.h>
#include <vector>
class CImage
{
private:
	int m_Width, m_Height;
	GLenum m_NRChannels;
	unsigned char* m_Data = nullptr;
public:
	CImage() = default;
	CImage(const char* vPath);
	CImage(int vWidth, int vHeight, int vNRCChannels, unsigned char* vData) : m_Width(vWidth), m_Height(vHeight), m_NRChannels(vNRCChannels), m_Data(vData) {};
	~CImage();
	const unsigned char* getData() const;
	int getWidth() const;
	int getHeight() const;
	int getNRChannels() const;
	void loadTextureFromImage(const tinygltf::Image& image);
};

