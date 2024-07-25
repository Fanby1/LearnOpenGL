#pragma once
#include <HiveConfig.h>

namespace GLTrain {
	class CShaderAlgConfig : public hiveConfig::CHiveConfig
	{
	public:
		CShaderAlgConfig();
		~CShaderAlgConfig() = default;
	private:
		void __setShaderAlgV() override;
		void __loadDefaltShaderV() override;
	};
}
