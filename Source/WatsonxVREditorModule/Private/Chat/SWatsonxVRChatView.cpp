

#include "SWatsonxVRChatView.h"
#include "WatsonxVRMessagingHandler.h"
#include <Tasks/WatsonxVRChatRequest.h>
#include <Management/WatsonxVRSettings.h>
#include <Utils/WatsonxVRHelper.h>
#include <WatsonxVRInternalFuncs.h>
#include <Interfaces/IPluginManager.h>
#include <Dom/JsonObject.h>
#include <Serialization/JsonWriter.h>
#include <Serialization/JsonReader.h>
#include <Serialization/JsonSerializer.h>
#include <Misc/FileHelper.h>
#include <Widgets/Layout/SScrollBox.h>
#include <Widgets/Input/STextComboBox.h>

void SWatsonxVRChatView::Construct(const FArguments& InArgs)
{
	SetSessionID(InArgs._SessionID);

	ModelsComboBox = SNew(STextComboBox).OptionsSource(&AvailableModels).ToolTipText(FText::FromString(TEXT("GPT Model")));

	for (const FName& ModelName : UWatsonxVRHelper::GetAvailableGPTModels())
	{
		AvailableModels.Add(MakeShared<FString>(ModelName.ToString()));

		if (ModelsComboBox.IsValid() && ModelName.IsEqual(UWatsonxVRHelper::ModelToName(EWatsonxVRChatModel::gpt35turbo)))
		{
			ModelsComboBox->SetSelectedItem(AvailableModels.Top());
		}
	}

	ChildSlot
	[
		ConstructContent()
	];

	LoadChatHistory();
}

SWatsonxVRChatView::~SWatsonxVRChatView()
{
	SaveChatHistory();
}

bool SWatsonxVRChatView::IsSendMessageEnabled() const
{
	return (!RequestReference.IsValid() || !UWatsonxVRTaskStatus::IsTaskActive(RequestReference.Get())) && !WatsonxVR::Internal::HasEmptyParam(
		InputTextBox->GetText());
}

bool SWatsonxVRChatView::IsClearChatEnabled() const
{
	return !WatsonxVR::Internal::HasEmptyParam(ChatItems);
}

FString SWatsonxVRChatView::GetHistoryPath() const
{
	return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("WatsonxVR"), SessionID.ToString() + TEXT(".json"));
}

void SWatsonxVRChatView::SetSessionID(const FName& NewSessionID)
{
	const FName NewValidSessionID = *FPaths::MakeValidFileName(NewSessionID.ToString());
	if (SessionID == NewValidSessionID)
	{
		return;
	}

	if (SessionID.IsNone())
	{
		SessionID = NewValidSessionID;
		return;
	}

	if (const FString OldPath = GetHistoryPath(); FPaths::FileExists(OldPath))
	{
		IFileManager::Get().Delete(*OldPath, true, true, true);
	}

	SessionID = NewValidSessionID;
	SaveChatHistory();
}

FName SWatsonxVRChatView::GetSessionID() const
{
	return SessionID;
}

void SWatsonxVRChatView::ClearChat()
{
	ChatItems.Empty();
	if (ChatBox.IsValid())
	{
		ChatBox->ClearChildren();
	}

	if (RequestReference.IsValid())
	{
		RequestReference->StopWatsonxVRTask();
		RequestReference.Reset();
	}
}

TSharedRef<SWidget> SWatsonxVRChatView::ConstructContent()
{
	constexpr float SlotPadding = 4.0f;

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot().Padding(SlotPadding).FillHeight(1.f)
		[
			SAssignNew(ChatScrollBox, SScrollBox)
			+ SScrollBox::Slot()
			[
				SAssignNew(ChatBox, SVerticalBox)
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
						if (IsSendMessageEnabled() && CommitType == ETextCommit::OnEnter)
						{
							HandleSendMessageButton(EWatsonxVRChatRole::User);
						}
					})
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Send")))
				.ToolTipText(FText::FromString(TEXT("Send Message")))
				.OnClicked(this, &SWatsonxVRChatView::HandleSendMessageButton, EWatsonxVRChatRole::User)
				.IsEnabled(this, &SWatsonxVRChatView::IsSendMessageEnabled)
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("System")))
				.ToolTipText(FText::FromString(TEXT("Send Message as System Context")))
				.OnClicked(this, &SWatsonxVRChatView::HandleSendMessageButton, EWatsonxVRChatRole::System)
				.IsEnabled(this, &SWatsonxVRChatView::IsSendMessageEnabled)
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				ModelsComboBox.ToSharedRef()
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Clear")))
				.ToolTipText(FText::FromString(TEXT("Clear Chat History")))
				.OnClicked(this, &SWatsonxVRChatView::HandleClearChatButton)
				.IsEnabled(this, &SWatsonxVRChatView::IsClearChatEnabled)
			]
		];
}

FReply SWatsonxVRChatView::HandleSendMessageButton(const EWatsonxVRChatRole Role)
{
	const SWatsonxVRChatItemPtr NewMessage = SNew(SWatsonxVRChatItem).MessageRole(Role).InputText(InputTextBox->GetText().ToString());

	ChatBox->AddSlot().AutoHeight()[NewMessage.ToSharedRef()];
	ChatItems.Add(NewMessage);

	if (Role == EWatsonxVRChatRole::System)
	{
		ChatScrollBox->ScrollToEnd();
		InputTextBox->SetText(FText::GetEmpty());

		return FReply::Handled();
	}

	const SWatsonxVRChatItemPtr AssistantMessage = SNew(SWatsonxVRChatItem).MessageRole(EWatsonxVRChatRole::Assistant).ScrollBox(ChatScrollBox);

	FWatsonxVRChatOptions Options;
	Options.Model = UWatsonxVRHelper::NameToModel(*(*ModelsComboBox->GetSelectedItem().Get()));
	Options.bStream = true;

	RequestReference = UWatsonxVRChatRequest::EditorTask(GetChatHistory(), Options);
	RequestReference->ProgressStarted.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UWatsonxVRMessagingHandler::ProcessUpdated);
	RequestReference->ProgressUpdated.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UWatsonxVRMessagingHandler::ProcessUpdated);
	RequestReference->ProcessCompleted.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UWatsonxVRMessagingHandler::ProcessCompleted);
	RequestReference->ErrorReceived.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UWatsonxVRMessagingHandler::ProcessCompleted);
	RequestReference->RequestFailed.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UWatsonxVRMessagingHandler::RequestFailed);
	RequestReference->RequestSent.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UWatsonxVRMessagingHandler::RequestSent);
	RequestReference->Activate();

	ChatBox->AddSlot().AutoHeight()[AssistantMessage.ToSharedRef()];
	ChatItems.Add(AssistantMessage);

	ChatScrollBox->ScrollToEnd();
	InputTextBox->SetText(FText::GetEmpty());

	return FReply::Handled();
}

FReply SWatsonxVRChatView::HandleClearChatButton()
{
	ClearChat();
	return FReply::Handled();
}

TArray<FWatsonxVRChatMessage> SWatsonxVRChatView::GetChatHistory() const
{
	TArray<FWatsonxVRChatMessage> Output{FWatsonxVRChatMessage(EWatsonxVRChatRole::System, GetDefaultSystemContext())};

	for (const SWatsonxVRChatItemPtr& Item : ChatItems)
	{
		FString RoleText = Item->GetRoleText();
		RoleText.RemoveFromEnd(TEXT(":"));
		Output.Add(FWatsonxVRChatMessage(*RoleText, Item->GetMessageText()));
	}

	return Output;
}

FString SWatsonxVRChatView::GetDefaultSystemContext() const
{
	if (const UWatsonxVRSettings* const Settings = UWatsonxVRSettings::Get(); Settings->bUseCustomSystemContext)
	{
		return Settings->CustomSystemContext;
	}

	FString SupportedModels;
	for (const FTextDisplayStringPtr& Model : AvailableModels)
	{
		SupportedModels.Append(*Model.Get() + ", ");
	}
	SupportedModels.RemoveFromEnd(", ");

	const TSharedPtr<IPlugin> PluginInterface = IPluginManager::Get().FindPlugin("WatsonxVR");

	const FString PluginShortName = "WatsonxVR";
	const FString EngineVersion = FString::Printf(TEXT("%d.%d"), ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION);

	const FStringFormatOrderedArguments Arguments_PluginInfo{
		EngineVersion, PluginShortName, PluginInterface->GetDescriptor().VersionName, PluginInterface->GetDescriptor().CreatedBy,
		PluginInterface->GetDescriptor().Description
	};

	const FString PluginInformation = FString::Format(
		TEXT("You are in the Unreal Engine {0} plugin {1} version {2}, which was developed by {3}. The description of WatsonxVR is: \"{4}\""),
		Arguments_PluginInfo);

	const FStringFormatOrderedArguments Arguments_SupportInfo{PluginInterface->GetDescriptor().DocsURL, PluginInterface->GetDescriptor().SupportURL,};

	const FString PluginSupport = FString::Format(TEXT("You can find the WatsonxVR documentation at {0} and support at {1}."), Arguments_SupportInfo);

	const FStringFormatOrderedArguments Arguments_Models{*ModelsComboBox->GetSelectedItem().Get(), SupportedModels};

	const FString ModelsInformation = FString::Format(
		TEXT("You're using the model {0} and WatsonxVR currently supports all these OpenAI Models: {1}."), Arguments_Models);

	const FStringFormatOrderedArguments Arguments_EngineDocumentation{EngineVersion};

	const FString EngineDocumentation_General = FString::Format(
		TEXT("You can find the Unreal Engine {0} general documentation at https://docs.unrealengine.com/{0}/en-US/."), Arguments_EngineDocumentation);
	const FString EngineDocumentation_CPP = FString::Format(
		TEXT("You can find the Unreal Engine {0} API documentation for C++ at https://docs.unrealengine.com/{0}/en-US/API/."),
		Arguments_EngineDocumentation);
	const FString EngineDocumentation_BP = FString::Format(
		TEXT("You can find the Unreal Engine {0} API documentation for Blueprints at https://docs.unrealengine.com/{0}/en-US/BlueprintAPI/."),
		Arguments_EngineDocumentation);

	const FStringFormatOrderedArguments Arguments_SystemContext{
		PluginInformation, PluginSupport, ModelsInformation, EngineDocumentation_General, EngineDocumentation_CPP, EngineDocumentation_BP
	};

	return FString::Format(
		TEXT("You are an assistant that will help with the development of projects in Unreal Engine in general.\n{0}\n{1}\n{2}\n{3}\n{4}\n{5}"),
		Arguments_SystemContext);
}

void SWatsonxVRChatView::LoadChatHistory()
{
	if (SessionID.IsNone())
	{
		return;
	}

	if (const FString LoadPath = GetHistoryPath(); FPaths::FileExists(LoadPath))
	{
		FString FileContent;
		if (!FFileHelper::LoadFileToString(FileContent, *LoadPath))
		{
			return;
		}

		TSharedPtr<FJsonObject> JsonParsed;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
		if (FJsonSerializer::Deserialize(Reader, JsonParsed))
		{
			const TArray<TSharedPtr<FJsonValue>> SessionData = JsonParsed->GetArrayField("Data");
			for (const TSharedPtr<FJsonValue>& Item : SessionData)
			{
				if (const TSharedPtr<FJsonObject> MessageItObj = Item->AsObject())
				{
					if (FString RoleString; MessageItObj->TryGetStringField("role", RoleString))
					{
						const EWatsonxVRChatRole Role = UWatsonxVRHelper::NameToRole(*RoleString);

						if (FString Message; MessageItObj->TryGetStringField("content", Message))
						{
							if (Role == EWatsonxVRChatRole::System && Message == GetDefaultSystemContext())
							{
								continue;
							}

							ChatItems.Emplace(SNew(SWatsonxVRChatItem).MessageRole(Role).InputText(Message).ScrollBox(ChatScrollBox));
						}
					}
				}
			}
		}
	}

	for (const SWatsonxVRChatItemPtr& Item : ChatItems)
	{
		ChatBox->AddSlot().AutoHeight()[Item.ToSharedRef()];
	}
}

void SWatsonxVRChatView::SaveChatHistory() const
{
	if (SessionID.IsNone() || ChatItems.IsEmpty())
	{
		return;
	}

	const TSharedPtr<FJsonObject> JsonRequest = MakeShared<FJsonObject>();

	TArray<TSharedPtr<FJsonValue>> Data;
	for (const FWatsonxVRChatMessage& Item : GetChatHistory())
	{
		Data.Add(Item.GetMessage());
	}

	JsonRequest->SetArrayField("Data", Data);

	FString RequestContentString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContentString);

	if (FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), Writer))
	{
		FFileHelper::SaveStringToFile(RequestContentString, *GetHistoryPath());
	}
}
