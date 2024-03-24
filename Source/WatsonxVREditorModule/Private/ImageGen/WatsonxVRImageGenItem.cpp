

#include "SWatsonxVRImageGenItem.h"
#include "SWatsonxVRImageGenItemData.h"
#include "WatsonxVRImageGetter.h"
#include <WatsonxVRInternalFuncs.h>
#include <Structures/WatsonxVRImageTypes.h>
#include <Tasks/WatsonxVRImageRequest.h>
#include <Utils/WatsonxVRHelper.h>
#include <Widgets/Layout/SScrollBox.h>

void SWatsonxVRImageGenItem::Construct(const FArguments& InArgs)
{
	Prompt = InArgs._Prompt;

	WatsonxVRImageGetterObject = NewObject<UWatsonxVRImageGetter>();
	WatsonxVRImageGetterObject->SetFlags(RF_Standalone);
	WatsonxVRImageGetterObject->OutScrollBox = InArgs._OutScrollBox;

	WatsonxVRImageGetterObject->OnImageGenerated.BindLambda([this](UTexture2D* const Texture)
	{
		if (Texture && ItemViewBox.IsValid())
		{
			ItemViewBox->AddSlot().AutoWidth()[SNew(SWatsonxVRImageGenItemData).Texture(Texture)];
		}
	});

	WatsonxVRImageGetterObject->OnStatusChanged.BindLambda([this](const FString& NewStatus)
	{
		if (WatsonxVR::Internal::HasEmptyParam(NewStatus) || !Status.IsValid())
		{
			return;
		}

		Status->SetText(FText::FromString(TEXT("Status: ") + NewStatus));
	});

	FWatsonxVRImageOptions Options;
	Options.Format = EWatsonxVRResponseFormat::b64_json;
	Options.Size = UWatsonxVRHelper::NameToSize(*InArgs._Size);
	Options.ImagesNum = FCString::Atoi(*InArgs._Num);

	RequestReference = UWatsonxVRImageRequest::EditorTask(Prompt, Options);

	RequestReference->ProcessCompleted.AddDynamic(WatsonxVRImageGetterObject.Get(), &UWatsonxVRImageGetter::ProcessCompleted);
	RequestReference->ErrorReceived.AddDynamic(WatsonxVRImageGetterObject.Get(), &UWatsonxVRImageGetter::ProcessCompleted);
	RequestReference->RequestFailed.AddDynamic(WatsonxVRImageGetterObject.Get(), &UWatsonxVRImageGetter::RequestFailed);
	RequestReference->RequestSent.AddDynamic(WatsonxVRImageGetterObject.Get(), &UWatsonxVRImageGetter::RequestSent);

	RequestReference->Activate();

	ChildSlot
	[
		ConstructContent()
	];
}

SWatsonxVRImageGenItem::~SWatsonxVRImageGenItem()
{
	if (RequestReference.IsValid())
	{
		RequestReference->StopWatsonxVRTask();
	}
}

TSharedRef<SWidget> SWatsonxVRImageGenItem::ConstructContent()
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
