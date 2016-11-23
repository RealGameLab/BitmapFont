#include "BitmapFontPrivatePCH.h"
#include "BMFontTextBlock.h"

UBMFontTextBlock::UBMFontTextBlock(class FObjectInitializer const &Initializer)
	: Font(nullptr)
	, MinDesiredWidth(0.f)
	, DesiredLineHeight(0.f)
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

void UBMFontTextBlock::SetShadowOffset(FVector2D InShadowOffset)
{
	ShadowOffset = InShadowOffset;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetShadowOffset(ShadowOffset);
	}
}

void UBMFontTextBlock::SetShadowColorAndOpacity(FLinearColor InShadowColorAndOpacity)
{
	ShadowColorAndOpacity = InShadowColorAndOpacity;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetShadowColorAndOpacity(InShadowColorAndOpacity);
	}
}

void UBMFontTextBlock::SetMinDesiredWidth(float InMinDesiredWidth)
{
	MinDesiredWidth = InMinDesiredWidth;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetMinDesiredWidth(InMinDesiredWidth);
	}
}

void UBMFontTextBlock::SetDesiredLineHeight(float InDesiredLineHeight)
{
	DesiredLineHeight = InDesiredLineHeight;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetDesiredLineHeight(InDesiredLineHeight);
	}
}

void UBMFontTextBlock::SetJustification(ETextJustify::Type InJustification)
{
	Justification = InJustification;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetJustification(InJustification);
	}
}

TSharedRef<SWidget> UBMFontTextBlock::RebuildWidget()
{
	MyTextBlock = SNew(SBMFontTextBlock);
	return MyTextBlock.ToSharedRef();
}

void UBMFontTextBlock::OnBindingChanged(const FName& Property)
{
	Super::OnBindingChanged(Property);

	if (MyTextBlock.IsValid())
	{
		static const FName TextProperty(TEXT("TextDelegate"));
		static const FName ColorAndOpacityProperty(TEXT("ColorAndOpacityDelegate"));
		static const FName ShadowColorAndOpacityProperty(TEXT("ShadowColorAndOpacityDelegate"));

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
		else if (Property == ShadowColorAndOpacityProperty)
		{
			TAttribute<FLinearColor> ShadowColorAndOpacityBinding = OPTIONAL_BINDING(FLinearColor, ShadowColorAndOpacity);
			MyTextBlock->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
		}
	}
}

void UBMFontTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FText> TextBinding = OPTIONAL_BINDING(FText, Text);
	TAttribute<FSlateColor> ColorAndOpacityBinding = OPTIONAL_BINDING(FSlateColor, ColorAndOpacity);
	TAttribute<FLinearColor> ShadowColorAndOpacityBinding = OPTIONAL_BINDING(FLinearColor, ShadowColorAndOpacity);

	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetText(TextBinding);
		MyTextBlock->SetFont(Font);
		MyTextBlock->SetColorAndOpacity(ColorAndOpacityBinding);
		MyTextBlock->SetShadowOffset(ShadowOffset);
		MyTextBlock->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
		MyTextBlock->SetMinDesiredWidth(MinDesiredWidth);
		MyTextBlock->SetDesiredLineHeight(DesiredLineHeight);

		Super::SynchronizeTextLayoutProperties(*MyTextBlock);
	}
}

void UBMFontTextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyTextBlock.Reset();
}