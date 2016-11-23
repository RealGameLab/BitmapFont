#pragma once

#include "ModuleManager.h"

class FBitmapFontModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};