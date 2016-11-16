#include "BitmapFontPrivatePCH.h"
#include "BMFontFileImportFactory.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Classes/Factories/TextureFactory.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"

DECLARE_LOG_CATEGORY_CLASS(BMFontFileImportFactory, Verbose, All);

UBMFontFileImportFactory::UBMFontFileImportFactory(class FObjectInitializer const & ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UFont::StaticClass();

	bEditorImport = true;
	bText = true;

	Formats.Add(TEXT("fnt;Bitmap font"));
}

UObject* UBMFontFileImportFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);
	UE_LOG(BMFontFileImportFactory, Verbose, TEXT("%s"), *CurrentFilename);
	UFont* const Font = NewObject<UFont>(InParent, InClass, InName, Flags);
	if (Font && InitBitmapFont(InParent, Font, Buffer))
	{
		FEditorDelegates::OnAssetPostImport.Broadcast(this, Font);
		return Font;
	}
	return nullptr;
}

bool UBMFontFileImportFactory::InitBitmapFont(UObject* InParent, UFont* Font, const TCHAR*& Buffer)
{
	Font->FontCacheType = EFontCacheType::Offline;
	Font->ImportOptions.bEnableLegacyMode = 0; // 不懂 lower quality antialiasing and larger glyph bounds， useful in debugging problems
	Font->IsRemapped = 1;

	TArray<FString> TextureFileNames;
	int32 MaxCharHeight = 1;
	TArray<FString> Lines;
	FString(Buffer).ParseIntoArrayLines(Lines);
	for (FString Line : Lines)
	{
		TArray<FString> Sections;
		FString(Line).ParseIntoArray(Sections, TEXT(" "));
		if (Sections.Num() <= 0)
		{
			continue;
		}

		if (Sections[0].Equals("info"))
		{
			for (int index = 1; index < Sections.Num(); index++)
			{
				TArray<FString> KVPair;
				FString(Sections[index]).ParseIntoArray(KVPair, TEXT("="));
				if (KVPair[0].Equals("face"))
				{
					Font->ImportOptions.FontName = KVPair[1];
				}
				else if (KVPair[0].Equals("size"))
				{
					// 不确定
					Font->ImportOptions.Height = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("bold"))
				{
					Font->ImportOptions.bEnableBold = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("italic"))
				{
					Font->ImportOptions.bEnableItalic = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("charset"))
				{

				}
				else if (KVPair[0].Equals("unicode"))
				{

				}
				else if (KVPair[0].Equals("stretchH"))
				{

				}
				else if (KVPair[0].Equals("smooth"))
				{

				}
				else if (KVPair[0].Equals("aa"))
				{
					Font->ImportOptions.bEnableAntialiasing = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("padding"))
				{
					// 内边距？不太确定
					TArray<FString> Values;
					FString(KVPair[1]).ParseIntoArray(Values, TEXT(","));
					if (Values.Num() == 4)
					{
						Font->ImportOptions.ExtendBoxTop = FCString::Atoi(*Values[0]);
						Font->ImportOptions.ExtendBoxBottom = FCString::Atoi(*Values[1]);
						Font->ImportOptions.ExtendBoxRight = FCString::Atoi(*Values[2]);
						Font->ImportOptions.ExtendBoxLeft = FCString::Atoi(*Values[3]);
					}
				}
				else if (KVPair[0].Equals("spacing"))
				{
					// 外边距？不太确定
					TArray<FString> Values;
					FString(KVPair[1]).ParseIntoArray(Values, TEXT(","));
					if (Values.Num() == 2)
					{
						Font->ImportOptions.XPadding = FCString::Atoi(*Values[0]);
						Font->ImportOptions.YPadding = FCString::Atoi(*Values[1]);
					}
				}
				else if (KVPair[0].Equals("outline"))
				{

				}
			}
		}
		else if (Sections[0].Equals("common"))
		{
			for (int index = 1; index < Sections.Num(); index++)
			{
				TArray<FString> KVPair;
				FString(Sections[index]).ParseIntoArray(KVPair, TEXT("="));
				if (KVPair[0].Equals("lineHeight"))
				{
				}
				else if (KVPair[0].Equals("base"))
				{
				}
				else if (KVPair[0].Equals("scaleW"))
				{
					Font->ImportOptions.TexturePageWidth = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("scaleH"))
				{
					Font->ImportOptions.TexturePageMaxHeight = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("pages"))
				{
				}
				else if (KVPair[0].Equals("packed"))
				{
				}
				else if (KVPair[0].Equals("alphaChnl"))
				{
					// 不确定
					FLinearColor Color = Font->ImportOptions.ForegroundColor;
					Color.A = FCString::Atoi(*KVPair[1]);
					Font->ImportOptions.ForegroundColor = Color;
				}
				else if (KVPair[0].Equals("redChnl"))
				{
					// 不确定
					FLinearColor Color = Font->ImportOptions.ForegroundColor;
					Color.R = FCString::Atoi(*KVPair[1]);
					Font->ImportOptions.ForegroundColor = Color;
				}
				else if (KVPair[0].Equals("greenChnl"))
				{
					// 不确定
					FLinearColor Color = Font->ImportOptions.ForegroundColor;
					Color.G = FCString::Atoi(*KVPair[1]);
					Font->ImportOptions.ForegroundColor = Color;
				}
				else if (KVPair[0].Equals("blueChnl"))
				{
					// 不确定
					FLinearColor Color = Font->ImportOptions.ForegroundColor;
					Color.B = FCString::Atoi(*KVPair[1]);
					Font->ImportOptions.ForegroundColor = Color;
				}
			}
		}
		else if (Sections[0].Equals("page"))
		{
			for (int index = 1; index < Sections.Num(); index++)
			{
				TArray<FString> KVPair;
				FString(Sections[index]).ParseIntoArray(KVPair, TEXT("="));
				if (KVPair[0].Equals("file"))
				{
					TextureFileNames.Add(KVPair[1].Replace(TEXT("\""), TEXT("")));
				}
			}
		}
		else if (Sections[0].Equals("chars"))
		{
			for (int index = 1; index < Sections.Num(); index++)
			{
				TArray<FString> KVPair;
				FString(Sections[index]).ParseIntoArray(KVPair, TEXT("="));
				if (KVPair[0].Equals("count"))
				{
					Font->NumCharacters = FCString::Atoi(*KVPair[1]);
				}
			}
		}
		else if (Sections[0].Equals("char"))
		{
			FFontCharacter Char;
			for (int index = 1; index < Sections.Num(); index++)
			{
				TArray<FString> KVPair;
				FString(Sections[index]).ParseIntoArray(KVPair, TEXT("="));
				if (KVPair[0].Equals("id"))
				{
					Font->CharRemap.Add(FCString::Atoi(*KVPair[1]), Font->Characters.Num());
				}
				else if (KVPair[0].Equals("x"))
				{
					Char.StartU = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("y"))
				{
					Char.StartV = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("width"))
				{
					Char.USize = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("height"))
				{
					Char.VSize = FCString::Atoi(*KVPair[1]);
					MaxCharHeight = FMath::Max(MaxCharHeight, Char.VSize);
				}
				else if (KVPair[0].Equals("xoffset"))
				{
				}
				else if (KVPair[0].Equals("yoffset"))
				{
					Char.VerticalOffset = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("xadvance"))
				{
				}
				else if (KVPair[0].Equals("page"))
				{
					Char.TextureIndex = FCString::Atoi(*KVPair[1]);
				}
				else if (KVPair[0].Equals("chnl"))
				{
				}
			}
			Font->Characters.Add(Char);
		}
		else if (Sections[0].Equals("kernings"))
		{
			for (int index = 1; index < Sections.Num(); index++)
			{
				TArray<FString> KVPair;
				FString(Sections[index]).ParseIntoArray(KVPair, TEXT("="));
				if (KVPair[0].Equals("count"))
				{
					
				}
			}
		}
		else if (Sections[0].Equals("kerning"))
		{
			for (int index = 1; index < Sections.Num(); index++)
			{
				TArray<FString> KVPair;
				FString(Sections[index]).ParseIntoArray(KVPair, TEXT("="));
				if (KVPair[0].Equals("first"))
				{

				}
				else if (KVPair[0].Equals("second"))
				{

				}
				else if (KVPair[0].Equals("amount"))
				{

				}
			}
		}
	}
	Font->MaxCharHeight.Add(MaxCharHeight);
	FString FontPath = FPaths::GetPath(CurrentFilename);
	for (FString Name : TextureFileNames)
	{
		
		UTexture2D *Texture = CreateTexture2D(InParent, FontPath + TEXT("\\") + Name);
		if (!Texture)
		{
			return false;
		}
		Font->Textures.Add(Texture);
	}
	return true;
}

UTexture2D* UBMFontFileImportFactory::CreateTexture2D(UObject* InParent, const FString &FullFilePath)
{
	UE_LOG(BMFontFileImportFactory, Verbose, TEXT("%s"), *FullFilePath);
	FString Extension = FPaths::GetExtension(FullFilePath).ToLower();
	FString TextureName = FPaths::GetBaseFilename(FullFilePath);
	FString NewPackageName = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) + TEXT("/") + TextureName;
	UPackage* Package = CreatePackage(nullptr, *NewPackageName);

	TArray<uint8> Buffer;
	if (!(FFileHelper::LoadFileToArray(Buffer, *FullFilePath) && Buffer.Num() > 0))
	{
		UE_LOG(BMFontFileImportFactory, Warning, TEXT("Unable to find Texture file %s"), *FullFilePath);
	}
	else
	{
		UTexture2D* Texture = nullptr;
		auto Factory = NewObject<UTextureFactory>();
		Factory->AddToRoot();
		Factory->SuppressImportOverwriteDialog();
		Factory->LODGroup = TEXTUREGROUP_UI;

		const uint8 *PtrBuffer = Buffer.GetData();
		Texture = (UTexture2D*)Factory->FactoryCreateBinary(UTexture2D::StaticClass(), Package, *TextureName, RF_Standalone | RF_Public, NULL, *Extension, PtrBuffer, PtrBuffer + Buffer.Num(), GWarn);
		if (Texture != nullptr)
		{
			Texture->AssetImportData->Update(FullFilePath);
			FAssetRegistryModule::AssetCreated(Texture);
			Package->SetDirtyFlag(true);
		}
		Factory->RemoveFromRoot();
		return Texture;
	}

	return nullptr;
}