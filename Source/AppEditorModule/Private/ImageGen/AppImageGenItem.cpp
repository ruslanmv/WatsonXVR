

#include "SAppImageGenItem.h"
#include "SAppImageGenItemData.h"
#include "AppImageGetter.h"
#include <AppInternalFuncs.h>
#include <Structures/AppImageTypes.h>
#include <Tasks/AppImageRequest.h>
#include <Utils/AppHelper.h>
#include <Widgets/Layout/SScrollBox.h>

void SAppImageGenItem::Construct(const FArguments& InArgs)
{
	Prompt = InArgs._Prompt;

	AppImageGetterObject = NewObject<UAppImageGetter>();
	AppImageGetterObject->SetFlags(RF_Standalone);
	AppImageGetterObject->OutScrollBox = InArgs._OutScrollBox;

	AppImageGetterObject->OnImageGenerated.BindLambda([this](UTexture2D* const Texture)
	{
		if (Texture && ItemViewBox.IsValid())
		{
			ItemViewBox->AddSlot().AutoWidth()[SNew(SAppImageGenItemData).Texture(Texture)];
		}
	});

	AppImageGetterObject->OnStatusChanged.BindLambda([this](const FString& NewStatus)
	{
		if (App::Internal::HasEmptyParam(NewStatus) || !Status.IsValid())
		{
			return;
		}

		Status->SetText(FText::FromString(TEXT("Status: ") + NewStatus));
	});

	FAppImageOptions Options;
	Options.Format = EAppResponseFormat::b64_json;
	Options.Size = UAppHelper::NameToSize(*InArgs._Size);
	Options.ImagesNum = FCString::Atoi(*InArgs._Num);

	RequestReference = UAppImageRequest::EditorTask(Prompt, Options);

	RequestReference->ProcessCompleted.AddDynamic(AppImageGetterObject.Get(), &UAppImageGetter::ProcessCompleted);
	RequestReference->ErrorReceived.AddDynamic(AppImageGetterObject.Get(), &UAppImageGetter::ProcessCompleted);
	RequestReference->RequestFailed.AddDynamic(AppImageGetterObject.Get(), &UAppImageGetter::RequestFailed);
	RequestReference->RequestSent.AddDynamic(AppImageGetterObject.Get(), &UAppImageGetter::RequestSent);

	RequestReference->Activate();

	ChildSlot
	[
		ConstructContent()
	];
}

SAppImageGenItem::~SAppImageGenItem()
{
	if (RequestReference.IsValid())
	{
		RequestReference->StopAppTask();
	}
}

TSharedRef<SWidget> SAppImageGenItem::ConstructContent()
{
	constexpr float SlotPadding = 4.0f;

#if ENGINE_MAJOR_VERSION < 5
    using FAppStyle = FEditorStyle;
#endif

	return SNew(SBox).Padding(SlotPadding)
		[
			SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Menu.Background"))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().Padding(SlotPadding).AutoHeight()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)).Text(FText::FromString(TEXT("Prompt: ") + Prompt))
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SAssignNew(Status, STextBlock).Text(FText::FromString(TEXT("Status: Sending request...")))
					]
				]
				+ SVerticalBox::Slot().Padding(SlotPadding).FillHeight(1.f)
				[
					SNew(SScrollBox).Orientation(Orient_Horizontal)
					+ SScrollBox::Slot()
					[
						SAssignNew(ItemViewBox, SHorizontalBox)
					]
				]
			]
		];
}
