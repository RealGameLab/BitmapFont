// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

class FBMFontTextBlockViewportClient;

class SBMFontTextBlock : public SCompoundWidget, public FGCObject
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

		SLATE_END_ARGS()

		virtual ~SBMFontTextBlock();

	/**
	* Construct this widget
	*
	* @param	InArgs	The declaration data for this widget
	*/
	void Construct(const FArguments& InArgs);

	/**
	* Gets the text assigned to this text block
	*
	* @return	This text block's text string
	*/
	const FText& GetText() const
	{
		return BoundText.Get();
	}

	/**
	* Sets the text for this text block
	*
	* @param	InText	The new text to display
	*/
	void SetText(const TAttribute<FText>& InText);
	void SetText(const FText& InText);

	/**
	* Sets the font used to draw the text
	*
	* @param	InFont	The new font to use
	*/
	void SetFont(const TAttribute<const UFont*>& InFont);

	/** See ColorAndOpacity attribute */
	void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity);

	/** See WrapTextAt attribute */
	void SetWrapTextAt(const TAttribute<float>& InWrapTextAt);

	/** See AutoWrapText attribute */
	void SetAutoWrapText(const TAttribute<bool>& InAutoWrapText);

	/** See Margin attribute */
	void SetMargin(const TAttribute<FMargin>& InMargin);

	// SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void CacheDesiredSize(float LayoutScaleMultiplier) override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	// End of SWidget interface

	// FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FGCObject interface

private:
	/** Single wrapped line */
	struct FWrappedLine
	{
		FWrappedLine(FText InText, FVector2D InTextSize)
			: Text(InText)
			, TextSize(InTextSize)
		{
		}

		FText Text;
		FVector2D TextSize;
	};

	/** Wrapping cache for this text block */
	class FWrappingCache
	{
	public:
		FWrappingCache()
			: CachedText()
			, CachedFont(nullptr)
			, CachedWrapTextAt(0.0f)
			, WrappedText()
			, WrappedSize(ForceInitToZero)
		{
		}

		void UpdateIfNeeded(const FText& InText, const UFont* InFont, const float InWrapTextAt);

		const TArray<FWrappedLine>& GetWrappedText() const
		{
			return WrappedText;
		}

		FVector2D GetWrappedSize() const
		{
			return WrappedSize;
		}

	private:
		FTextSnapshot CachedText;
		const UFont* CachedFont;
		float CachedWrapTextAt;

		TArray<FWrappedLine> WrappedText;
		FVector2D WrappedSize;
	};

	/** The text displayed in this text block */
	TAttribute<FText> BoundText;

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

	/** Viewport client used by this text block */
	TSharedPtr<FBMFontTextBlockViewportClient> ViewportClient;

	/** Viewport used by this text block */
	TSharedPtr<FSceneViewport> Viewport;

	/** Cached wrapping data */
	FWrappingCache WrappingCache;

	/** The last known size of the widget from the previous OnPaint, used to guess at an auto-wrapping width in CacheDesiredSize */
	mutable FVector2D WidgetCachedSize;
};
