

#include "SWatsonxVRImageGenView.h"
#include "SWatsonxVRImageGenItem.h"
#include <Utils/WatsonxVRHelper.h>
#include <WatsonxVRInternalFuncs.h>
#include <Widgets/Layout/SScrollBox.h>
#include <Widgets/Input/STextComboBox.h>

void SWatsonxVRImageGenView::Construct(const FArguments& InArgs)
{
	InitializeImageNumOptions();
	ImageNumComboBox = SNew(STextComboBox).OptionsSource(&ImageNum).InitiallySelectedItem(ImageNum[0]).ToolTipText(
		FText::FromString(TEXT("Number of Generated Images")));

	InitializeImageSizeOptions();
	ImageSizeComboBox = SNew(STextComboBox).OptionsSource(&ImageSize).InitiallySelectedItem(ImageSize[0]).ToolTipText(
		FText::FromString(TEXT("Size of Generated Images")));

	ChildSlot
	[
		ConstructContent()
	];
}

TSharedRef<SWidget> SWatsonxVRImageGenView::ConstructContent()
{
	constexpr float SlotPadding = 4.0f;

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot().Padding(SlotPadding).FillHeight(1.f)
		[
			SAssignNew(ViewScrollBox, SScrollBox)
			+ SScrollBox::Slot()
			[
				SAssignNew(ViewBox, SVerticalBox)
			]
		]
		+ SVerticalBox::Slot().Padding(SlotPadding).AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().Padding(SlotPadding).FillWidth(1.f)
			[
				SAssignNew(InputTextBox, SEditableTextBox).AllowContextMenu(true).IsReadOnly(false).OnTextCommitted_Lambda(
					[this]([[maybe_unused]] const FText& Text, ETextCommit::Type CommitType)
					{
						if (IsSendRequestEnabled() && CommitType == ETextCommit::OnEnter)
						{
							HandleSendRequestButton();
						}
					})
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Generate")))
				.ToolTipText(FText::FromString(TEXT("Request Images Generation")))
				.OnClicked(this, &SWatsonxVRImageGenView::HandleSendRequestButton)
				.IsEnabled(this, &SWatsonxVRImageGenView::IsSendRequestEnabled)
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				ImageNumComboBox.ToSharedRef()
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				ImageSizeComboBox.ToSharedRef()
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Clear")))
				.ToolTipText(FText::FromString(TEXT("Clear Generation History")))
				.OnClicked(this, &SWatsonxVRImageGenView::HandleClearViewButton)
				.IsEnabled(this, &SWatsonxVRImageGenView::IsClearViewEnabled)
			]
		];
}

FReply SWatsonxVRImageGenView::HandleSendRequestButton()
{
	ViewBox->AddSlot().AutoHeight()[SNew(SWatsonxVRImageGenItem)
	.OutScrollBox(ViewScrollBox)
	.Prompt(InputTextBox->GetText().ToString())
	.Num(*ImageNumComboBox->GetSelectedItem().Get())
	.Size(*ImageSizeComboBox->GetSelectedItem().Get())];

	ViewScrollBox->ScrollToEnd();
	InputTextBox->SetText(FText::GetEmpty());

	return FReply::Handled();
}

bool SWatsonxVRImageGenView::IsSendRequestEnabled() const
{
	return !WatsonxVR::Internal::HasEmptyParam(InputTextBox->GetText());
}

FReply SWatsonxVRImageGenView::HandleClearViewButton()
{
	ViewBox->ClearChildren();
	return FReply::Handled();
}

bool SWatsonxVRImageGenView::IsClearViewEnabled() const
{
	return ViewBox->NumSlots() > 0;
}

void SWatsonxVRImageGenView::InitializeImageNumOptions()
{
	constexpr uint8 MaxNum = 10u;
	for (uint8 Iterator = 1u; Iterator <= MaxNum; ++Iterator)
	{
		ImageNum.Add(MakeShared<FString>(FString::FromInt(Iterator)));
	}
}

void SWatsonxVRImageGenView::InitializeImageSizeOptions()
{
	ImageSize.Add(MakeShared<FString>(UWatsonxVRHelper::SizeToName(EWatsonxVRImageSize::x256).ToString()));
	ImageSize.Add(MakeShared<FString>(UWatsonxVRHelper::SizeToName(EWatsonxVRImageSize::x512).ToString()));
	ImageSize.Add(MakeShared<FString>(UWatsonxVRHelper::SizeToName(EWatsonxVRImageSize::x1024).ToString()));
}
