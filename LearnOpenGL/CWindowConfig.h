#pragma once
#include "CGLBaseConfig.h"

class GLTRAINVER3_API CWindowConfig : public CGLBaseConfig
{
public:
	CWindowConfig(const std::string& vFilePath = "");
	~CWindowConfig() = default;
	// any wtf value is ok, the CWindow will check and provide default.
	bool isInit() const { return m_IsInit; }
	void initV() override;
	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
	int getPosX() const { return m_PosX; }
	int getPosY() const { return m_PosY; }
	std::string getTitle() const { return m_Title; }
	int getMajVer() const { return m_MajVer; }
	int getMinVer() const { return m_MinVer; }
	bool isCore() const { return m_IsCore; }

private:
	int m_Width, m_Height, m_PosX, m_PosY, m_MajVer, m_MinVer;
	bool m_IsCore, m_IsInit;
	std::string m_Title = "";

	void __defineAttributesV() override;
	void __setValFromConfig();
};
