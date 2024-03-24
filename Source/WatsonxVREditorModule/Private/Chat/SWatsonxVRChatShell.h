

#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

using FNamePtr = TSharedPtr<FName>;

class SWatsonxVRChatShell final : public SCompoundWidget
{
public:
	SLATE_USER_ARGS(SWatsonxVRChatShell)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SWatsonxVRChatShell() override;

private:
	TSharedRef<SWidget> ConstructContent();

	void InitializeChatSessionOptions();
	void InitializeChatSession(const FNamePtr& InItem);

	TSharedPtr<class SBox> ShellBox;
	TSharedPtr<class SWatsonxVRChatView> CurrentView;

	TSharedPtr<class SListView<FNamePtr>> ChatSessionListView;
	TArray<FNamePtr> ChatSessions;

	TSharedRef<ITableRow> OnGenerateChatSessionRow(FNamePtr InItem, const TSharedRef<STableViewBase>& OwnerTable);

	void OnChatSessionSelectionChanged(const FNamePtr InItem, ESelectInfo::Type SelectInfo);
	void OnChatSessionNameChanged(const FNamePtr InItem, const FName& NewName);
	void OnChatSessionDoubleClicked(const FNamePtr InItem);
	FReply OnChatSessionKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
};
