#pragma once
#include <string>
#include "def.h"
#include "HiveConfig.h"

class GLTRAINVER3_API CGLBaseConfig : public hiveConfig::CHiveConfig
{
public:
	CGLBaseConfig(const std::string& vFilePath = "");
	virtual ~CGLBaseConfig() = default;

	virtual void initV() = 0;

protected:
	std::string m_FilePath = "";
	void _logNoExist(const std::string vVarName);

	template<typename T>
	void _setTypeVal(T& vMember, const std::string& vVarName) {
		std::optional<T> s;
		s.reset();
		s = getAttribute<T>(vVarName);
		if (!s.has_value()) _logNoExist(vVarName);
		else vMember = s.value();
	}

};

