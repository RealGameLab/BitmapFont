#pragma once

#include "Editor/UnrealEd/Classes/Factories/Factory.h"
#include "Runtime/Engine/Classes/Engine/Font.h"
#include "Runtime/ImageWrapper/Public/ImageWrapper.h"
#include "BMFontFileImportFactory.generated.h"

UCLASS()
class UBMFontFileImportFactory : public UFactory 
{
	GENERATED_UCLASS_BODY()
	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn);
	//~Begin UFactory Interface

private:
	bool InitBitmapFont(UFont* Font, const TCHAR*& Buffer);
	UTexture2D* LoadTexture2DFromFile(const FString& FullFilePath, EImageFormat::Type ImageFormat);
	EImageFormat::Type GetTextureType(const FString& TextureName);
};
