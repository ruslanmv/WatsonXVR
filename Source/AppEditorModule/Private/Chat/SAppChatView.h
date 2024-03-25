

#pragma once

#include <CoreMinimal.h>
#include <Structures/AppChatTypes.h>
#include <Widgets/SCompoundWidget.h>
#include "SAppChatItem.h"

class SAppChatView final : public SCompoundWidget
{
public:
	SLATE_USER_ARGS(SAppChatView)
			: _SessionID(NAME_None)
		{
		}

		SLATE_ARGUMENT(FName, SessionID)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SAppChatView() override;

	bool IsSendMessageEnabled() const;
	bool IsClearChatEnabled() const;
	FString GetHistoryPath() const;

	void SetSessionID(const FName& NewSessionID);
	FName GetSessionID() const;

	void ClearChat();

private:
	TSharedRef<SWidget> ConstructContent();

	FReply HandleSendMessageButton(const EAppChatRole Role);
	FReply HandleClearChatButton();

	TArray<FAppChatMessage> GetChatHistory() const;
	FString GetDefaultSystemContext() const;

	void LoadChatHistory();
	void SaveChatHistory() const;

	FName SessionID;

	TSharedPtr<class SVerticalBox> ChatBox;
	TArray<SAppChatItemPtr> ChatItems;
	TSharedPtr<class SScrollBox> ChatScrollBox;

	TSharedPtr<class SEditableTextBox> InputTextBox;

	TSharedPtr<class STextComboBox> ModelsComboBox;
	TArray<FTextDisplayStringPtr> AvailableModels;

	TWeakObjectPtr<class UAppChatRequest> RequestReference;
};
