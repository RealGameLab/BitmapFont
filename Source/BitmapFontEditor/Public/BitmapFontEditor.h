#pragma once

#include "ModuleManager.h"

class FBitmapFontEditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};