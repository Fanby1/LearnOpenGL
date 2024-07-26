#include "CImage.h"

CImage::CImage(const char* vPath)
{
	stbi_set_flip_vertically_on_load(true);
	int Format;
	m_Data = stbi_load(vPath, &m_Width, &m_Height, &Format, 0);
	m_NRChannels = Format;
}

CImage::~CImage()
{
	stbi_image_free(m_Data);
}

const unsigned char* CImage::getData() const
{
	return m_Data;
}

int CImage::getWidth() const {
	return m_Width;
}

int CImage::getHeight() const {
	return m_Height;
}

int CImage::getNRChannels() const {
	return m_NRChannels;
}

void CImage::loadTextureFromImage(const tinygltf::Image& vImage)
{
    m_Width = vImage.width;
    m_Height = vImage.height;
    m_NRChannels = vImage.component;
	m_Data = new unsigned char[vImage.image.size()];
	memcpy(m_Data, vImage.image.data(), vImage.image.size());
}

