#include "CImage.h"

CImage::CImage(const char* vPath)
{
	stbi_set_flip_vertically_on_load(true);
	m_Data = stbi_load(vPath, &m_Width, &m_Height, &m_NRChannels, 0);
}

CImage::~CImage()
{
	stbi_image_free(m_Data);
}

unsigned char* CImage::getData() const
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

