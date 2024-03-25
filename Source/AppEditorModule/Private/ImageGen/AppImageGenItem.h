
#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

class SAppImageGenItem final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAppImageGenItem) : _OutScrollBox(), _Prompt(), _Num(), _Size()
		{
		}

		SLATE_ARGUMENT(TSharedPtr<class SScrollBox>, OutScrollBox)
		SLATE_ARGUMENT(FString, Prompt)
		SLATE_ARGUMENT(FString, Num)
		SLATE_ARGUMENT(FString, Size)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SAppImageGenItem() override;

	TWeakObjectPtr<class UAppImageGetter> AppImageGetterObject;

private:
	TSharedRef<SWidget> ConstructContent();

	FString Prompt;

	TSharedPtr<class STextBlock> Status;
	TSharedPtr<class SHorizontalBox> ItemViewBox;

	TWeakObjectPtr<class UAppImageRequest> RequestReference;
};

using SAppImageGenItemPtr = TSharedPtr<SAppImageGenItem>;
