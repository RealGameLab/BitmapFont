// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BitmapFontPrivatePCH.h"
#include "SBMFontTextBlock.h"
#include "SceneViewport.h"

class FBMFontTextBlockViewportClient : public FViewportClient
{
public:
	FBMFontTextBlockViewportClient()
		: LinesToDraw()
		, FontPtr(nullptr)
	{
	}

	void PrepareToDraw(FViewport* Viewport, const TArray<SBMFontTextBlock::FWrappedLine>& InLinesToDraw, const UFont* InFontPtr, const FLinearColor& InDrawColor, const FLinearColor& InShadowDrawColor, const FVector2D& InShadowDrawOffset, const FMargin& InDrawMargin)
	{
		LinesToDraw = InLinesToDraw;
		FontPtr = InFontPtr;
		DrawColor = InDrawColor;
		ShadowDrawColor = InShadowDrawColor;
		ShadowDrawOffset = InShadowDrawOffset;
		DrawMargin = InDrawMargin;

		Viewport->Invalidate();
		Viewport->InvalidateDisplay();
	}

	/** FViewportClient interface */
	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override
	{
		Canvas->Clear(FLinearColor::Transparent);

		if (LinesToDraw.Num() > 0 && FontPtr)
		{
			FVector2D CanvasPos = FVector2D(DrawMargin.Left, DrawMargin.Top);

			for (const SBMFontTextBlock::FWrappedLine& WrappedLine : LinesToDraw)
			{
				FCanvasTextItem TextItem(CanvasPos, WrappedLine.Text, FontPtr, DrawColor);
				TextItem.EnableShadow(ShadowDrawColor, ShadowDrawOffset);
				TextItem.BlendMode = SE_BLEND_AlphaComposite;

				Canvas->DrawItem(TextItem);

				CanvasPos.Y += WrappedLine.TextSize.Y;
			}
		}
	}

private:
	TArray<SBMFontTextBlock::FWrappedLine> LinesToDraw;
	const UFont* FontPtr;
	FLinearColor DrawColor;
	FLinearColor ShadowDrawColor;
	FVector2D ShadowDrawOffset;
	FMargin DrawMargin;
};

SBMFontTextBlock::~SBMFontTextBlock()
{
}

void SBMFontTextBlock::Construct(const FArguments& InArgs)
{
	BoundText = InArgs._Text;
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

void SBMFontTextBlock::SetText(const TAttribute<FText>& InText)
{
	BoundText = InText;
}

void SBMFontTextBlock::SetText(const FText& InText)
{
	BoundText = InText;
}

void SBMFontTextBlock::SetFont(const TAttribute<const UFont*>& InFont)
{
	Font = InFont;
}

void SBMFontTextBlock::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
}

void SBMFontTextBlock::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	WrapTextAt = InWrapTextAt;
}

void SBMFontTextBlock::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	AutoWrapText = InAutoWrapText;
}

void SBMFontTextBlock::SetMargin(const TAttribute<FMargin>& InMargin)
{
	Margin = InMargin;
}

int32 SBMFontTextBlock::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	WidgetCachedSize = AllottedGeometry.Size;

	const bool bEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect::Type DrawEffects = bEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	const UFont* const FontPtr = Font.Get(nullptr);
	const FLinearColor& DrawColor = ColorAndOpacity.Get(FLinearColor::White).GetColor(InWidgetStyle);
	const FLinearColor& ShadowDrawColor = ShadowColorAndOpacity.Get(FLinearColor::Black);
	const FVector2D& DrawShadowOffset = ShadowOffset.Get(FVector2D::ZeroVector);
	const FMargin& DrawMargin = Margin.Get(FMargin());

	ViewportClient->PrepareToDraw(Viewport.Get(), WrappingCache.GetWrappedText(), FontPtr, DrawColor, ShadowDrawColor, DrawShadowOffset, DrawMargin);

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

	const FMargin& DrawMargin = Margin.Get(FMargin());
	WrappingWidth -= DrawMargin.GetTotalSpaceAlong<Orient_Horizontal>();

	WrappingWidth = FMath::Max(0.0f, WrappingWidth);

	const FText& TextToDraw = BoundText.Get(FText::GetEmpty());
	const UFont* const FontPtr = Font.Get(nullptr);
	WrappingCache.UpdateIfNeeded(TextToDraw, FontPtr, WrappingWidth);
}

FVector2D SBMFontTextBlock::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	const FMargin& DrawMargin = Margin.Get(FMargin());
	return WrappingCache.GetWrappedSize() + DrawMargin.GetDesiredSize();
}

void SBMFontTextBlock::AddReferencedObjects(FReferenceCollector& Collector)
{
	const UFont* FontPtr = Font.Get(nullptr);
	if (FontPtr)
	{
		Collector.AddReferencedObject(FontPtr);
	}
}

void SBMFontTextBlock::FWrappingCache::UpdateIfNeeded(const FText& InText, const UFont* InFont, const float InWrapTextAt)
{
	const bool bTextIsIdentical = CachedText.IdenticalTo(InText) && CachedText.IsDisplayStringEqualTo(InText);
	const bool bIsDirty = !bTextIsIdentical || CachedFont != InFont || CachedWrapTextAt != InWrapTextAt;
	if (bIsDirty)
	{
		CachedText = FTextSnapshot(InText);
		CachedFont = InFont;
		CachedWrapTextAt = InWrapTextAt;

		WrappedText.Empty();
		WrappedSize = FVector2D::ZeroVector;

		if (!InText.IsEmpty() && InFont)
		{
			const bool bIsWrapping = InWrapTextAt > 0.0f;
			if (bIsWrapping)
			{
// 				FTextSizingParameters TextSizingParameters(0.0f, 0.0f, InWrapTextAt, 0.0f, InFont);
// 				TArray<FWrappedStringElement> WrappedStringData;
// 				UCanvas::WrapString(TextSizingParameters, 0.0f, *InText.ToString(), WrappedStringData);
// 				
// 				for (const FWrappedStringElement& Entry : WrappedStringData)
// 				{
// 					WrappedText.Add(FWrappedLine(FText::FromString(Entry.Value), Entry.LineExtent));
// 					WrappedSize += Entry.LineExtent;
// 				}
			}
			else
			{
				FTextSizingParameters TextSizingParameters(InFont, 1.0f, 1.0f);
				UCanvas::CanvasStringSize(TextSizingParameters, *InText.ToString());

				const FVector2D LineExtent(TextSizingParameters.DrawXL, TextSizingParameters.DrawYL);
				WrappedText.Add(FWrappedLine(InText, LineExtent));
				WrappedSize += LineExtent;
			}
		}
	}
}
