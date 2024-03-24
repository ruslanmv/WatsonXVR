

#pragma once

#include <CoreMinimal.h>
#include <Structures/WatsonxVRChatTypes.h>
#include <Widgets/SCompoundWidget.h>
#include "SWatsonxVRChatItem.h"

class SWatsonxVRChatView final : public SCompoundWidget
{
public:
	SLATE_USER_ARGS(SWatsonxVRChatView)
			: _SessionID(NAME_None)
		{
		}

		SLATE_ARGUMENT(FName, SessionID)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SWatsonxVRChatView() override;

	bool IsSendMessageEnabled() const;
	bool IsClearChatEnabled() const;
	FString GetHistoryPath() const;

	void SetSessionID(const FName& NewSessionID);
	FName GetSessionID() const;

	void ClearChat();

private:
	TSharedRef<SWidget> ConstructContent();

	FReply HandleSendMessageButton(const EWatsonxVRChatRole Role);
	FReply HandleClearChatButton();

	TArray<FWatsonxVRChatMessage> GetChatHistory() const;
	FString GetDefaultSystemContext() const;

	void LoadChatHistory();
	void SaveChatHistory() const;

	FName SessionID;

	TSharedPtr<class SVerticalBox> ChatBox;
	TArray<SWatsonxVRChatItemPtr> ChatItems;
	TSharedPtr<class SScrollBox> ChatScrollBox;

	TSharedPtr<class SEditableTextBox> InputTextBox;

	TSharedPtr<class STextComboBox> ModelsComboBox;
	TArray<FTextDisplayStringPtr> AvailableModels;

	TWeakObjectPtr<class UWatsonxVRChatRequest> RequestReference;
};
