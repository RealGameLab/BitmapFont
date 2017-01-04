#pragma once

#include "ModuleManager.h"
#include "Widgets/SViewport.h"

class FBitmapFontModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};