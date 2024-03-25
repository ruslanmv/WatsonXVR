

#include "SAppImageGenView.h"
#include "SAppImageGenItem.h"
#include <Utils/AppHelper.h>
#include <AppInternalFuncs.h>
#include <Widgets/Layout/SScrollBox.h>
#include <Widgets/Input/STextComboBox.h>

void SAppImageGenView::Construct(const FArguments& InArgs)
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

TSharedRef<SWidget> SAppImageGenView::ConstructContent()
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
				.OnClicked(this, &SAppImageGenView::HandleSendRequestButton)
				.IsEnabled(this, &SAppImageGenView::IsSendRequestEnabled)
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
				.OnClicked(this, &SAppImageGenView::HandleClearViewButton)
				.IsEnabled(this, &SAppImageGenView::IsClearViewEnabled)
			]
		];
}

FReply SAppImageGenView::HandleSendRequestButton()
{
	ViewBox->AddSlot().AutoHeight()[SNew(SAppImageGenItem)
	.OutScrollBox(ViewScrollBox)
	.Prompt(InputTextBox->GetText().ToString())
	.Num(*ImageNumComboBox->GetSelectedItem().Get())
	.Size(*ImageSizeComboBox->GetSelectedItem().Get())];

	ViewScrollBox->ScrollToEnd();
	InputTextBox->SetText(FText::GetEmpty());

	return FReply::Handled();
}

bool SAppImageGenView::IsSendRequestEnabled() const
{
	return !App::Internal::HasEmptyParam(InputTextBox->GetText());
}

FReply SAppImageGenView::HandleClearViewButton()
{
	ViewBox->ClearChildren();
	return FReply::Handled();
}

bool SAppImageGenView::IsClearViewEnabled() const
{
	return ViewBox->NumSlots() > 0;
}

void SAppImageGenView::InitializeImageNumOptions()
{
	constexpr uint8 MaxNum = 10u;
	for (uint8 Iterator = 1u; Iterator <= MaxNum; ++Iterator)
	{
		ImageNum.Add(MakeShared<FString>(FString::FromInt(Iterator)));
	}
}

void SAppImageGenView::InitializeImageSizeOptions()
{
	ImageSize.Add(MakeShared<FString>(UAppHelper::SizeToName(EAppImageSize::x256).ToString()));
	ImageSize.Add(MakeShared<FString>(UAppHelper::SizeToName(EAppImageSize::x512).ToString()));
	ImageSize.Add(MakeShared<FString>(UAppHelper::SizeToName(EAppImageSize::x1024).ToString()));
}
