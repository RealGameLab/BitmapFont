#pragma once

#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/TextWidgetTypes.h"
#include "SBMFontTextBlock.h"
#include "BMFontTextBlock.generated.h"

UCLASS()
class UBMFontTextBlock : public UTextLayoutWidget
{
	GENERATED_UCLASS_BODY()
public:
	//  Ù–‘≤Œ’’ TextBlock.h
	UPROPERTY(EditAnywhere, Category = Content, meta = (MultiLine = "true"))
	FText Text;

	UPROPERTY()
	FGetText TextDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateColor ColorAndOpacity;

	UPROPERTY()
	FGetSlateColor ColorAndOpacityDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	UFont* Font;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FVector2D ShadowOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (DisplayName = "Shadow Color"))
	FLinearColor ShadowColorAndOpacity;
 
	UPROPERTY()
	FGetLinearColor ShadowColorAndOpacityDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	bool bOutLine;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FLinearColor OutLineColorAndOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	float MinDesiredWidth;
	
	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "GetText (Text)"))
	FText GetText() const;

	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "SetText (Text)"))
	void SetText(FText InText);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetFont(UFont* InFontInfo);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetColorAndOpacity(FSlateColor InColorAndOpacity);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOpacity(float InOpacity);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetShadowColorAndOpacity(FLinearColor InShadowColorAndOpacity);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetShadowOffset(FVector2D InShadowOffset);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOutLine(bool InOutLine);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOutLineColorAndOpacity(FLinearColor InOutLineColor);
// 	UFUNCTION(BlueprintCallable, Category = "Appearance")
// 	void SetJustification(ETextJustify::Type InJustification);

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface
protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnBindingChanged(const FName& Property) override;
	//~ End UWidget Interface

protected:
	TSharedPtr<SBMFontTextBlock> MyTextBlock;
};