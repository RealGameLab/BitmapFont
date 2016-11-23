#pragma once

class FBMFontTextBlockViewportClient;

class SBMFontTextBlock : public SCompoundWidget//, public FGCObject
{
	friend class FBMFontTextBlockViewportClient;

public:
	SLATE_BEGIN_ARGS(SBMFontTextBlock)
		: _Text(FText::GetEmpty())
		, _Font(nullptr)
		, _ColorAndOpacity(FLinearColor::White)
		, _ShadowOffset(FVector2D::ZeroVector)
		, _ShadowColorAndOpacity(FLinearColor::Black)
		, _WrapTextAt(0.0f)
		, _AutoWrapText(false)
		, _Margin()
		, _MinDesiredWidth(0.f)
		, _DesiredLineHeight(0.f)
	{}

		/** The text displayed in this text block */
		SLATE_ATTRIBUTE(FText, Text)

		/** Sets the font used to draw the text */
		SLATE_ATTRIBUTE(const UFont*, Font)

		/** Text color and opacity */
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

		/** Drop shadow offset in pixels */
		SLATE_ATTRIBUTE(FVector2D, ShadowOffset)

		/** Shadow color and opacity */
		SLATE_ATTRIBUTE(FLinearColor, ShadowColorAndOpacity)

		/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
		SLATE_ATTRIBUTE(float, WrapTextAt)

		/** Whether to wrap text automatically based on the widget's computed horizontal space.  IMPORTANT: Using automatic wrapping can result
		in visual artifacts, as the the wrapped size will computed be at least one frame late!  Consider using WrapTextAt instead.  The initial
		desired size will not be clamped.  This works best in cases where the text block's size is not affecting other widget's layout. */
		SLATE_ATTRIBUTE(bool, AutoWrapText)

		/** The amount of blank space left around the edges of text area. */
		SLATE_ATTRIBUTE(FMargin, Margin)

		SLATE_ATTRIBUTE(float, MinDesiredWidth)

		SLATE_ATTRIBUTE(float, DesiredLineHeight)

		SLATE_ATTRIBUTE(ETextWrappingPolicy, WrappingPolicy)

		SLATE_ATTRIBUTE(float, LineHeightPercentage)

		SLATE_ATTRIBUTE(ETextJustify::Type, Justification)
	SLATE_END_ARGS()

	virtual ~SBMFontTextBlock();

	void Construct(const FArguments& InArgs);

	const FText& GetText() const
	{
		return Text.Get();
	}

	void SetText(const TAttribute<FText>& InText)
	{
		Text = InText;
	}

	void SetText(const FText& InText)
	{
		Text = InText;
	}

	void SetFont(const TAttribute<const UFont*>& InFont)
	{
		Font = InFont;
	}

	void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
	{
		ColorAndOpacity = InColorAndOpacity;
	}

	void SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset)
	{
		ShadowOffset = InShadowOffset;
	}

	void SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
	{
		ShadowColorAndOpacity = InShadowColorAndOpacity;
	}

	void SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
	{
		WrapTextAt = InWrapTextAt;
	}

	void SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
	{
		AutoWrapText = InAutoWrapText;
	}

	void SetMargin(const TAttribute<FMargin>& InMargin)
	{
		Margin = InMargin;
	}

	void SetMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth)
	{
		MinDesiredWidth = InMinDesiredWidth;
	}

	void SetDesiredLineHeight(const TAttribute<float>& InDesiredLineHeight)
	{
		DesiredLineHeight = InDesiredLineHeight;
	}

	void SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
	{
		WrappingPolicy = InWrappingPolicy;
	}

	void SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
	{
		LineHeightPercentage = InLineHeightPercentage;
	}

	void SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
	{
		Justification = InJustification;
	}

	void SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
	{
	}
	void SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
	{
	}

	// SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void CacheDesiredSize(float LayoutScaleMultiplier) override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	// End of SWidget interface

private:
	/** Wrapping cache for this text block */
	struct FWrappingCache
	{
		FWrappingCache()
			: Font(nullptr)
			, WrapTextAt(0.0f)
			, WrappedSize(ForceInitToZero)
			, DesiredLineHeight(0.f)
			, MinDesiredWidth(0.f)
		{
		}

		void UpdateIfNeeded(const FText& InText, const UFont* InFont, float InWrapTextAt, float InMinDesiredWidth, float InDesiredLineHeight);

		FTextSnapshot TextShot;
		const UFont* Font;
		float WrapTextAt;
		float DesiredLineHeight;
		float MinDesiredWidth;
		FVector2D WrappedSize;
		TArray<FWrappedStringElement> WrappedText;
	};

	/** The text displayed in this text block */
	TAttribute<FText> Text;

	/** The font used to draw the text */
	TAttribute<const UFont*> Font;

	/** Text color and opacity */
	TAttribute<FSlateColor> ColorAndOpacity;

	/** Drop shadow offset in pixels */
	TAttribute<FVector2D> ShadowOffset;

	/** Shadow color and opacity */
	TAttribute<FLinearColor> ShadowColorAndOpacity;

	/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
	TAttribute<float> WrapTextAt;

	/** True if we're wrapping text automatically based on the computed horizontal space for this widget */
	TAttribute<bool> AutoWrapText;

	/** The amount of blank space left around the edges of text area. */
	TAttribute<FMargin> Margin;

	TAttribute<float> MinDesiredWidth;

	TAttribute<float> DesiredLineHeight;

	TAttribute<ETextWrappingPolicy> WrappingPolicy;

	TAttribute<float> LineHeightPercentage;

	TAttribute<ETextJustify::Type> Justification;

	/** Viewport client used by this text block */
	TSharedPtr<FBMFontTextBlockViewportClient> ViewportClient;

	/** Viewport used by this text block */
	TSharedPtr<FSceneViewport> Viewport;

	/** Cached wrapping data */
	FWrappingCache WrappingCache;

	/** The last known size of the widget from the previous OnPaint, used to guess at an auto-wrapping width in CacheDesiredSize */
	mutable FVector2D WidgetCachedSize;
};
