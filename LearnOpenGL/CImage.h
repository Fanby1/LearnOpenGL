#pragma once
#include "stb_image.h"
class CImage
{
private:
	int m_Width, m_Height, m_NRChannels;
	unsigned char* m_Data;
public:
	CImage(const char* vPath);
	~CImage();
	unsigned char* getData() const;
	int getWidth() const;
	int getHeight() const;
	int getNRChannels() const;
};

