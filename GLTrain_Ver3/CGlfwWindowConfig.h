#pragma once
#include <HiveConfig.h>

namespace GLTrain {
	class CGLFWWindowConfig : public hiveConfig::CHiveConfig
	{
	public:
		CGLFWWindowConfig();
		~CGLFWWindowConfig() = default;
	private:
		void __setSelfWindowV() override;
		void __loadDefaultWindowV() override;
	};
}

