#include "BitmapFontPrivatePCH.h"
#include "BMFontTextBlock.h"
#include "Runtime/Slate/Public/Widgets/SInvalidationPanel.h"

UBMFontTextBlock::UBMFontTextBlock(class FObjectInitializer const &Initializer)
{
	
}

FText UBMFontTextBlock::GetText() const
{
	if (MyTextBlock.IsValid())
	{
		return MyTextBlock->GetText();
	}

	return Text;
}

void UBMFontTextBlock::SetText(FText InText)
{
	Text = InText;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetText(Text);
	}
}

void UBMFontTextBlock::SetColorAndOpacity(FSlateColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UBMFontTextBlock::SetOpacity(float InOpacity)
{
	FLinearColor CurrentColor = ColorAndOpacity.GetSpecifiedColor();
	CurrentColor.A = InOpacity;

	SetColorAndOpacity(FSlateColor(CurrentColor));
}

void UBMFontTextBlock::SetFont(UFont* InFontInfo)
{
	Font = InFontInfo;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetFont(Font);
	}
}

// void UBMFontTextBlock::SetJustification(ETextJustify::Type InJustification)
// {
// 	Justification = InJustification;
// 	if (MyTextBlock.IsValid())
// 	{
// 		MyTextBlock->SetJustification(Justification);
// 	}
// }

// void UBMFontTextBlock::SetShadowOffset(FVector2D InShadowOffset)
// {
// 	ShadowOffset = InShadowOffset;
// 	if (MyTextBlock.IsValid())
// 	{
// 		MyTextBlock->SetShadowOffset(ShadowOffset);
// 	}
// }
// 
// void UBMFontTextBlock::SetShadowColorAndOpacity(FLinearColor InShadowColorAndOpacity)
// {
// 	ShadowColorAndOpacity = InShadowColorAndOpacity;
// 	if (MyTextBlock.IsValid())
// 	{
// 		MyTextBlock->SetShadowColorAndOpacity(InShadowColorAndOpacity);
// 	}
// }

TSharedRef<SWidget> UBMFontTextBlock::RebuildWidget()
{
	if (bWrapWithInvalidationPanel && !IsDesignTime())
	{
		TSharedPtr<SWidget> RetWidget = SNew(SInvalidationPanel)
			[
				SAssignNew(MyTextBlock, SBMFontTextBlock)
			];
		return RetWidget.ToSharedRef();
	}
	else
	{
		MyTextBlock = SNew(SBMFontTextBlock);
		return MyTextBlock.ToSharedRef();
	}
}

void UBMFontTextBlock::OnBindingChanged(const FName& Property)
{
	Super::OnBindingChanged(Property);

	if (MyTextBlock.IsValid())
	{
		static const FName TextProperty(TEXT("TextDelegate"));
		static const FName ColorAndOpacityProperty(TEXT("ColorAndOpacityDelegate"));
//		static const FName ShadowColorAndOpacityProperty(TEXT("ShadowColorAndOpacityDelegate"));

		if (Property == TextProperty)
		{
			TAttribute<FText> TextBinding = OPTIONAL_BINDING(FText, Text);
			MyTextBlock->SetText(TextBinding);
		}
		else if (Property == ColorAndOpacityProperty)
		{
			TAttribute<FSlateColor> ColorAndOpacityBinding = OPTIONAL_BINDING(FSlateColor, ColorAndOpacity);
			MyTextBlock->SetColorAndOpacity(ColorAndOpacityBinding);
		}
// 		else if (Property == ShadowColorAndOpacityProperty)
// 		{
// 			TAttribute<FLinearColor> ShadowColorAndOpacityBinding = OPTIONAL_BINDING(FLinearColor, ShadowColorAndOpacity);
// 			MyTextBlock->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
// 		}
	}
}

void UBMFontTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FText> TextBinding = OPTIONAL_BINDING(FText, Text);
	TAttribute<FSlateColor> ColorAndOpacityBinding = OPTIONAL_BINDING(FSlateColor, ColorAndOpacity);
//	TAttribute<FLinearColor> ShadowColorAndOpacityBinding = OPTIONAL_BINDING(FLinearColor, ShadowColorAndOpacity);

	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetText(TextBinding);
		MyTextBlock->SetFont(Font);
		MyTextBlock->SetColorAndOpacity(ColorAndOpacityBinding);
//		MyTextBlock->SetShadowOffset(ShadowOffset);
//		MyTextBlock->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
//		MyTextBlock->SetMinDesiredWidth(MinDesiredWidth);

		//Super::SynchronizeTextLayoutProperties(*MyTextBlock);
	}
}

