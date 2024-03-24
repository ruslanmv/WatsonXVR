

#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>
#include <Structures/WatsonxVRChatTypes.h>

static const FName NewSessionName = TEXT("New Session");

class SWatsonxVRChatItem final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWatsonxVRChatItem) : _MessageRole(), _InputText(), _ScrollBox()
		{
		}

		SLATE_ARGUMENT(EWatsonxVRChatRole, MessageRole)
		SLATE_ARGUMENT(FString, InputText)
		SLATE_ARGUMENT(TSharedPtr<class SScrollBox>, ScrollBox)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FString GetRoleText() const;
	FString GetMessageText() const;

	TWeakObjectPtr<class UWatsonxVRMessagingHandler> MessagingHandlerObject;

private:
	TSharedRef<SWidget> ConstructContent();

	EWatsonxVRChatRole MessageRole;
	FString InputText;

	TSharedPtr<class STextBlock> Role;
	TSharedPtr<class SMultiLineEditableText> Message;
};

using SWatsonxVRChatItemPtr = TSharedPtr<SWatsonxVRChatItem>;
