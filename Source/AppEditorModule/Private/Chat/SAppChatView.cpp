

#include "SAppChatView.h"
#include "AppMessagingHandler.h"
#include <Tasks/AppChatRequest.h>
#include <Management/AppSettings.h>
#include <Utils/AppHelper.h>
#include <AppInternalFuncs.h>
#include <Interfaces/IPluginManager.h>
#include <Dom/JsonObject.h>
#include <Serialization/JsonWriter.h>
#include <Serialization/JsonReader.h>
#include <Serialization/JsonSerializer.h>
#include <Misc/FileHelper.h>
#include <Widgets/Layout/SScrollBox.h>
#include <Widgets/Input/STextComboBox.h>

void SAppChatView::Construct(const FArguments& InArgs)
{
	SetSessionID(InArgs._SessionID);

	ModelsComboBox = SNew(STextComboBox).OptionsSource(&AvailableModels).ToolTipText(FText::FromString(TEXT("GPT Model")));

	for (const FName& ModelName : UAppHelper::GetAvailableGPTModels())
	{
		AvailableModels.Add(MakeShared<FString>(ModelName.ToString()));

		if (ModelsComboBox.IsValid() && ModelName.IsEqual(UAppHelper::ModelToName(EAppChatModel::gpt35turbo)))
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

SAppChatView::~SAppChatView()
{
	SaveChatHistory();
}

bool SAppChatView::IsSendMessageEnabled() const
{
	return (!RequestReference.IsValid() || !UAppTaskStatus::IsTaskActive(RequestReference.Get())) && !App::Internal::HasEmptyParam(
		InputTextBox->GetText());
}

bool SAppChatView::IsClearChatEnabled() const
{
	return !App::Internal::HasEmptyParam(ChatItems);
}

FString SAppChatView::GetHistoryPath() const
{
	return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("App"), SessionID.ToString() + TEXT(".json"));
}

void SAppChatView::SetSessionID(const FName& NewSessionID)
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

FName SAppChatView::GetSessionID() const
{
	return SessionID;
}

void SAppChatView::ClearChat()
{
	ChatItems.Empty();
	if (ChatBox.IsValid())
	{
		ChatBox->ClearChildren();
	}

	if (RequestReference.IsValid())
	{
		RequestReference->StopAppTask();
		RequestReference.Reset();
	}
}

TSharedRef<SWidget> SAppChatView::ConstructContent()
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
							HandleSendMessageButton(EAppChatRole::User);
						}
					})
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Send")))
				.ToolTipText(FText::FromString(TEXT("Send Message")))
				.OnClicked(this, &SAppChatView::HandleSendMessageButton, EAppChatRole::User)
				.IsEnabled(this, &SAppChatView::IsSendMessageEnabled)
			]
			+ SHorizontalBox::Slot().Padding(SlotPadding).AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("System")))
				.ToolTipText(FText::FromString(TEXT("Send Message as System Context")))
				.OnClicked(this, &SAppChatView::HandleSendMessageButton, EAppChatRole::System)
				.IsEnabled(this, &SAppChatView::IsSendMessageEnabled)
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
				.OnClicked(this, &SAppChatView::HandleClearChatButton)
				.IsEnabled(this, &SAppChatView::IsClearChatEnabled)
			]
		];
}

FReply SAppChatView::HandleSendMessageButton(const EAppChatRole Role)
{
	const SAppChatItemPtr NewMessage = SNew(SAppChatItem).MessageRole(Role).InputText(InputTextBox->GetText().ToString());

	ChatBox->AddSlot().AutoHeight()[NewMessage.ToSharedRef()];
	ChatItems.Add(NewMessage);

	if (Role == EAppChatRole::System)
	{
		ChatScrollBox->ScrollToEnd();
		InputTextBox->SetText(FText::GetEmpty());

		return FReply::Handled();
	}

	const SAppChatItemPtr AssistantMessage = SNew(SAppChatItem).MessageRole(EAppChatRole::Assistant).ScrollBox(ChatScrollBox);

	FAppChatOptions Options;
	Options.Model = UAppHelper::NameToModel(*(*ModelsComboBox->GetSelectedItem().Get()));
	Options.bStream = true;

	RequestReference = UAppChatRequest::EditorTask(GetChatHistory(), Options);
	RequestReference->ProgressStarted.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UAppMessagingHandler::ProcessUpdated);
	RequestReference->ProgressUpdated.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UAppMessagingHandler::ProcessUpdated);
	RequestReference->ProcessCompleted.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UAppMessagingHandler::ProcessCompleted);
	RequestReference->ErrorReceived.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UAppMessagingHandler::ProcessCompleted);
	RequestReference->RequestFailed.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UAppMessagingHandler::RequestFailed);
	RequestReference->RequestSent.AddDynamic(AssistantMessage->MessagingHandlerObject.Get(), &UAppMessagingHandler::RequestSent);
	RequestReference->Activate();

	ChatBox->AddSlot().AutoHeight()[AssistantMessage.ToSharedRef()];
	ChatItems.Add(AssistantMessage);

	ChatScrollBox->ScrollToEnd();
	InputTextBox->SetText(FText::GetEmpty());

	return FReply::Handled();
}

FReply SAppChatView::HandleClearChatButton()
{
	ClearChat();
	return FReply::Handled();
}

TArray<FAppChatMessage> SAppChatView::GetChatHistory() const
{
	TArray<FAppChatMessage> Output{FAppChatMessage(EAppChatRole::System, GetDefaultSystemContext())};

	for (const SAppChatItemPtr& Item : ChatItems)
	{
		FString RoleText = Item->GetRoleText();
		RoleText.RemoveFromEnd(TEXT(":"));
		Output.Add(FAppChatMessage(*RoleText, Item->GetMessageText()));
	}

	return Output;
}

FString SAppChatView::GetDefaultSystemContext() const
{
	if (const UAppSettings* const Settings = UAppSettings::Get(); Settings->bUseCustomSystemContext)
	{
		return Settings->CustomSystemContext;
	}

	FString SupportedModels;
	for (const FTextDisplayStringPtr& Model : AvailableModels)
	{
		SupportedModels.Append(*Model.Get() + ", ");
	}
	SupportedModels.RemoveFromEnd(", ");

	const TSharedPtr<IPlugin> PluginInterface = IPluginManager::Get().FindPlugin("App");

	const FString PluginShortName = "App";
	const FString EngineVersion = FString::Printf(TEXT("%d.%d"), ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION);

	const FStringFormatOrderedArguments Arguments_PluginInfo{
		EngineVersion, PluginShortName, PluginInterface->GetDescriptor().VersionName, PluginInterface->GetDescriptor().CreatedBy,
		PluginInterface->GetDescriptor().Description
	};

	const FString PluginInformation = FString::Format(
		TEXT("You are in the Unreal Engine {0} plugin {1} version {2}, which was developed by {3}. The description of App is: \"{4}\""),
		Arguments_PluginInfo);

	const FStringFormatOrderedArguments Arguments_SupportInfo{PluginInterface->GetDescriptor().DocsURL, PluginInterface->GetDescriptor().SupportURL,};

	const FString PluginSupport = FString::Format(TEXT("You can find the App documentation at {0} and support at {1}."), Arguments_SupportInfo);

	const FStringFormatOrderedArguments Arguments_Models{*ModelsComboBox->GetSelectedItem().Get(), SupportedModels};

	const FString ModelsInformation = FString::Format(
		TEXT("You're using the model {0} and App currently supports all these OpenAI Models: {1}."), Arguments_Models);

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

void SAppChatView::LoadChatHistory()
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
						const EAppChatRole Role = UAppHelper::NameToRole(*RoleString);

						if (FString Message; MessageItObj->TryGetStringField("content", Message))
						{
							if (Role == EAppChatRole::System && Message == GetDefaultSystemContext())
							{
								continue;
							}

							ChatItems.Emplace(SNew(SAppChatItem).MessageRole(Role).InputText(Message).ScrollBox(ChatScrollBox));
						}
					}
				}
			}
		}
	}

	for (const SAppChatItemPtr& Item : ChatItems)
	{
		ChatBox->AddSlot().AutoHeight()[Item.ToSharedRef()];
	}
}

void SAppChatView::SaveChatHistory() const
{
	if (SessionID.IsNone() || ChatItems.IsEmpty())
	{
		return;
	}

	const TSharedPtr<FJsonObject> JsonRequest = MakeShared<FJsonObject>();

	TArray<TSharedPtr<FJsonValue>> Data;
	for (const FAppChatMessage& Item : GetChatHistory())
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
