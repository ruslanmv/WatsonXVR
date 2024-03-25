

#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>
#include <Structures/AppChatTypes.h>

static const FName NewSessionName = TEXT("New Session");

class SAppChatItem final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAppChatItem) : _MessageRole(), _InputText(), _ScrollBox()
		{
		}

		SLATE_ARGUMENT(EAppChatRole, MessageRole)
		SLATE_ARGUMENT(FString, InputText)
		SLATE_ARGUMENT(TSharedPtr<class SScrollBox>, ScrollBox)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FString GetRoleText() const;
	FString GetMessageText() const;

	TWeakObjectPtr<class UAppMessagingHandler> MessagingHandlerObject;

private:
	TSharedRef<SWidget> ConstructContent();

	EAppChatRole MessageRole;
	FString InputText;

	TSharedPtr<class STextBlock> Role;
	TSharedPtr<class SMultiLineEditableText> Message;
};

using SAppChatItemPtr = TSharedPtr<SAppChatItem>;
