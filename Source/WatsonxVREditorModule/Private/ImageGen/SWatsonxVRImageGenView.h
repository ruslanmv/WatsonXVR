

#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

class SWatsonxVRImageGenView final : public SCompoundWidget
{
public:
	SLATE_USER_ARGS(SWatsonxVRImageGenView)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	bool IsSendRequestEnabled() const;
	bool IsClearViewEnabled() const;

private:
	TSharedRef<SWidget> ConstructContent();

	FReply HandleSendRequestButton();
	FReply HandleClearViewButton();

	void InitializeImageNumOptions();
	void InitializeImageSizeOptions();

	TSharedPtr<class SVerticalBox> ViewBox;
	TSharedPtr<class SScrollBox> ViewScrollBox;

	TSharedPtr<class SEditableTextBox> InputTextBox;

	TSharedPtr<class STextComboBox> ImageNumComboBox;
	TArray<FTextDisplayStringPtr> ImageNum;

	TSharedPtr<class STextComboBox> ImageSizeComboBox;
	TArray<FTextDisplayStringPtr> ImageSize;
};
