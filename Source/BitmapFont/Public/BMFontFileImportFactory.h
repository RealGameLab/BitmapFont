#pragma once

#include "BMFontFileImportFactory.generated.h"

UCLASS()
class UBMFontFileImportFactory : public UFactory, public FReimportHandler
{
	GENERATED_UCLASS_BODY()
	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn);
	//~Begin UFactory Interface

	//~ Begin FReimportHandler Interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	//~ End FReimportHandler Interface

private:
	bool ImportBitmapFont(UFont* Font, const TCHAR* Buffer, const FString &PackageName, const FString &FileName);
	UTexture2D* CreateTexture2D(const FString &FullFilePath, const FString &PackageName);
};
