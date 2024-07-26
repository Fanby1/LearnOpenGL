#pragma once
#include <string>
#include "def.h"
#include "HiveConfig.h"

class GLTRAINVER3_API CWindowConfig : public hiveConfig::CHiveConfig
{
public:
	CWindowConfig(const std::string& vFilePath = "");
	~CWindowConfig() = default;
	// any wtf value is ok, the CWindow will check and provide default.
	bool isInit() const { return m_isInit; }
	void init();
	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
	int getPosX() const { return m_PosX; }
	int getPosY() const { return m_PosY; }
	std::string getTitle() const { return m_Title; }
	int getMajVer() const { return m_MajVer; }
	int getMinVer() const { return m_MinVer; }
	bool isCore() const { return m_isCore; }

private:
	std::string m_FilePath = "";

	int m_Width, m_Height, m_PosX, m_PosY, m_MajVer, m_MinVer;
	bool m_isCore, m_isInit;
	std::string m_Title = "";

	void __readConfigFromFile();
	void __defineAttributesV();
	void __setValFromConfig();
	template<typename T>void __setTypeVal(T& vMemberName, const std::string& vType);
	void __logNoExist(const std::string vType);
};
