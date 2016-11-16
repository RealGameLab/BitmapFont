// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BitmapFontPrivatePCH.h"
#include "SBMFontTextBlock.h"
#include "SceneViewport.h"

class FBMFontTextBlockViewportClient : public FViewportClient
{
public:
	FBMFontTextBlockViewportClient();

	void PrepareToDraw(FViewport* Viewport, const TArray<FWrappedStringElement> &InText, const UFont *InFont, const FLinearColor &InColor, const FVector2D &InShadowOffset, const FLinearColor &InShadowColor, const FMargin &InMargin, float InScale);

	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override;

private:
	TArray<FWrappedStringElement> Text;
	const UFont* Font;
	FLinearColor Color;
	FVector2D ShadowOffset;
	FLinearColor ShadowColor;
	FMargin Margin;
	FVector2D Scale;
};

FBMFontTextBlockViewportClient::FBMFontTextBlockViewportClient()
	: Font(nullptr)
{
}

void FBMFontTextBlockViewportClient::PrepareToDraw(FViewport* Viewport, const TArray<FWrappedStringElement> &InText, const UFont *InFont, const FLinearColor &InColor, const FVector2D &InShadowOffset, const FLinearColor &InShadowColor, const FMargin &InMargin, float InScale)
{
	Text = InText;
	Font = InFont;
	Color = InColor;
	ShadowColor = InShadowColor;
	ShadowOffset = InShadowOffset;
	Margin = InMargin;
	Scale = FVector2D(InScale, InScale);

	Viewport->Invalidate();
	Viewport->InvalidateDisplay();
}

void FBMFontTextBlockViewportClient::Draw(FViewport* Viewport, FCanvas* Canvas)
{
	Canvas->Clear(FLinearColor::Transparent);

	if (Text.Num() > 0 && Font)
	{
		FVector2D CanvasPos = FVector2D(Margin.Left, Margin.Top);

		for (const FWrappedStringElement& Line : Text)
		{
			FCanvasTextItem TextItem(CanvasPos, FText::FromString(Line.Value), Font, Color);
			TextItem.EnableShadow(ShadowColor, ShadowOffset * Scale);
			TextItem.BlendMode = SE_BLEND_AlphaBlend;
			TextItem.Scale = Scale;

			Canvas->DrawItem(TextItem);

			CanvasPos.Y += Line.LineExtent.Y;
		}
	}
}

void SBMFontTextBlock::FWrappingCache::UpdateIfNeeded(const FText& InText, const UFont* InFont, const float InWrapTextAt)
{
	bool bTextIsIdentical = TextShot.IdenticalTo(InText) && TextShot.IsDisplayStringEqualTo(InText);
	if (!bTextIsIdentical || Font != InFont || WrapTextAt != InWrapTextAt)
 	{
		TextShot = FTextSnapshot(InText);
		Font = InFont;
		WrapTextAt = InWrapTextAt;

		WrappedText.Empty();
		WrappedSize = FVector2D::ZeroVector;

		if (!InText.IsEmpty() && InFont)
		{
			if (InWrapTextAt > 0.0f)
			{
				FTextSizingParameters TextSizingParameters(0.0f, 0.0f, InWrapTextAt, 0.0f, InFont);
				FCanvasWordWrapper Wrapper;
				UCanvas::WrapString(Wrapper, TextSizingParameters, 0.0f, *InText.ToString(), WrappedText);
				WrappedSize.X = InWrapTextAt;
				for (const FWrappedStringElement &Line : WrappedText)
				{
					WrappedSize.Y += Line.LineExtent.Y;
				}
			}
			else
			{
				FTextSizingParameters TextSizingParameters(InFont, 1.f, 1.f);
				UCanvas::CanvasStringSize(TextSizingParameters, *InText.ToString());
				FWrappedStringElement Line = FWrappedStringElement(*InText.ToString(), TextSizingParameters.DrawXL, TextSizingParameters.DrawYL);
				WrappedText.Add(Line);
				WrappedSize = Line.LineExtent;
			}
		}
	}
}

SBMFontTextBlock::~SBMFontTextBlock()
{
}

void SBMFontTextBlock::Construct(const FArguments& InArgs)
{
	Text = InArgs._Text;
	Font = InArgs._Font;
	ColorAndOpacity = InArgs._ColorAndOpacity;
	ShadowOffset = InArgs._ShadowOffset;
	ShadowColorAndOpacity = InArgs._ShadowColorAndOpacity;
	WrapTextAt = InArgs._WrapTextAt;
	AutoWrapText = InArgs._AutoWrapText;
	Margin = InArgs._Margin;

	WidgetCachedSize = FVector2D::ZeroVector;

	TSharedPtr<SViewport> ViewportWidget;
	ChildSlot
		[
			SAssignNew(ViewportWidget, SViewport)
			.EnableGammaCorrection(true)
			.EnableBlending(true)
			.ShowEffectWhenDisabled(true)
			.IgnoreTextureAlpha(false)
		];

	ViewportClient = MakeShareable(new FBMFontTextBlockViewportClient());
	Viewport = MakeShareable(new FSceneViewport(ViewportClient.Get(), ViewportWidget));

	ViewportWidget->SetViewportInterface(Viewport.ToSharedRef());
}

int32 SBMFontTextBlock::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	WidgetCachedSize = AllottedGeometry.Size;

	const bool bEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect::Type DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	ViewportClient->PrepareToDraw(Viewport.Get(), 
		WrappingCache.WrappedText, 
		Font.Get(nullptr), 
		ColorAndOpacity.Get(FLinearColor::White).GetColor(InWidgetStyle), 
		ShadowOffset.Get(FVector2D::ZeroVector),
		ShadowColorAndOpacity.Get(FLinearColor::Black),
		Margin.Get(FMargin()), 
		AllottedGeometry.Scale);

	LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bEnabled);

	return LayerId;
}

void SBMFontTextBlock::CacheDesiredSize(float LayoutScaleMultiplier)
{
	// Text wrapping can either be used defined (WrapTextAt), automatic (bAutoWrapText and CachedSize), 
	// or a mixture of both. Take whichever has the smallest value (>1)
	const bool bAutoWrapText = AutoWrapText.Get(false);
	float WrappingWidth = WrapTextAt.Get(0.0f);
	if (bAutoWrapText && WidgetCachedSize.X >= 1.0f)
	{
		WrappingWidth = (WrappingWidth >= 1.0f) ? FMath::Min(WrappingWidth, WidgetCachedSize.X) : WidgetCachedSize.X;
	}
	WrappingWidth -= Margin.Get(FMargin()).GetTotalSpaceAlong<Orient_Horizontal>();
	WrappingWidth = FMath::Max(0.0f, WrappingWidth);

	WrappingCache.UpdateIfNeeded(Text.Get(FText::GetEmpty()), Font.Get(nullptr), WrappingWidth);
	
	SCompoundWidget::CacheDesiredSize(LayoutScaleMultiplier);
}

FVector2D SBMFontTextBlock::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	FVector2D WrappedSize = WrappingCache.WrappedSize;
	FVector2D MarginSize = Margin.Get(FMargin()).GetDesiredSize();
	FVector2D ShadowSize = ShadowOffset.Get(FVector2D::ZeroVector).GetAbs() * LayoutScaleMultiplier;
	return WrappedSize + MarginSize + ShadowSize;
}

// void SBMFontTextBlock::AddReferencedObjects(FReferenceCollector& Collector)
// {
// 	const UFont* FontPtr = Font.Get(nullptr);
// 	if (FontPtr)
// 	{
// 		Collector.AddReferencedObject(FontPtr);
// 	}
// }


// 
// void SBMFontTextBlock::SetJustification(const TAttribute<ETextJustify::Type>& Justification)
// {
// 
// }
// 
// void SBMFontTextBlock::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& WrappingPolicy)
// {
// 
// }
// 
// void SBMFontTextBlock::SetLineHeightPercentage(const TAttribute<float>& LineHeightPercentage)
// {
// 
// }
// 
// void SBMFontTextBlock::SetTextShapingMethod(TOptional<ETextShapingMethod>)
// {
// }
// void SBMFontTextBlock::SetTextFlowDirection(TOptional<ETextFlowDirection>)
// {
// }