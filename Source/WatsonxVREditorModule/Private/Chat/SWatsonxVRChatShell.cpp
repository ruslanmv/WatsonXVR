

#include "SWatsonxVRChatShell.h"
#include "SWatsonxVRChatView.h"
#include <Widgets/Views/SListView.h>
#include <Widgets/Text/SInlineEditableTextBlock.h>
#include <Widgets/Input/STextEntryPopup.h>

using FOnChatSessionNameChanged = TDelegate<void(FNamePtr, const FName&)>;

class SWatsonxVRChatSessionOption : public STableRow<FNamePtr>
{
public:
	SLATE_BEGIN_ARGS(SWatsonxVRChatSessionOption)
		{
		}

		SLATE_EVENT(FOnChatSessionNameChanged, OnNameChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, const FNamePtr& InItem)
	{
		OnNameChanged = InArgs._OnNameChanged;
		Item = InItem;

		STableRow<FNamePtr>::Construct(
			STableRow<FNamePtr>::FArguments().Padding(8.f).Content()[SAssignNew(SessionName, STextBlock)
			.Text(this, &SWatsonxVRChatSessionOption::GetName)
			.ToolTipText(this, &SWatsonxVRChatSessionOption::GetName)], InOwnerTableView);
	}

	FText GetName() const
	{
		return FText::FromName(Item.IsValid() ? *Item : TEXT("Invalid"));
	}

	void EnableEditMode()
	{
		const TSharedRef<STextEntryPopup> TextEntry = SNew(STextEntryPopup).Label(FText::FromString("Rename Session")).OnTextCommitted(
				this, &SWatsonxVRChatSessionOption::OnNameCommited);

		FSlateApplication& SlateApp = FSlateApplication::Get();

		SlateApp.PushMenu(AsShared(), FWidgetPath(), TextEntry, SlateApp.GetCursorPos(), FPopupTransitionEffect::TypeInPopup);
	}

private:
	FNamePtr Item;
	FOnChatSessionNameChanged OnNameChanged;

	TSharedPtr<STextBlock> SessionName;

	void OnNameCommited(const FText& NewText, const ETextCommit::Type CommitInfo)
	{
		if (!SessionName.IsValid())
		{
			return;
		}

		if (CommitInfo == ETextCommit::OnEnter)
		{
			OnNameChanged.ExecuteIfBound(Item, FName(*NewText.ToString()));
			*Item = *NewText.ToString();

			FSlateApplication::Get().DismissAllMenus();
		}
		else if (CommitInfo == ETextCommit::OnCleared)
		{
			FSlateApplication::Get().DismissAllMenus();
		}
	}
};

void SWatsonxVRChatShell::Construct([[maybe_unused]] const FArguments&)
{
	ChildSlot
	[
		ConstructContent()
	];

	InitializeChatSessionOptions();
}

SWatsonxVRChatShell::~SWatsonxVRChatShell() = default;

TSharedRef<SWidget> SWatsonxVRChatShell::ConstructContent()
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().FillWidth(0.2f)
		[
			SAssignNew(ChatSessionListView, SListView<FNamePtr>)
			.ListItemsSource(&ChatSessions)
			.OnGenerateRow(this, &SWatsonxVRChatShell::OnGenerateChatSessionRow)
			.OnSelectionChanged(this, &SWatsonxVRChatShell::OnChatSessionSelectionChanged)
			.SelectionMode(ESelectionMode::Single)
			.ClearSelectionOnClick(false)
			.OnMouseButtonDoubleClick(this, &SWatsonxVRChatShell::OnChatSessionDoubleClicked)
			.OnKeyDownHandler(this, &SWatsonxVRChatShell::OnChatSessionKeyDown)

		]
		+ SHorizontalBox::Slot().FillWidth(0.8f)
		[
			SAssignNew(ShellBox, SBox).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		];
}

void SWatsonxVRChatShell::InitializeChatSessionOptions()
{
	ChatSessions.Empty();

	if (const FString SessionsPath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("WatsonxVR")); FPaths::DirectoryExists(SessionsPath))
	{
		TArray<FString> FoundFiles;
		IFileManager::Get().FindFilesRecursive(FoundFiles, *SessionsPath, TEXT("*.json"), true, false, true);

		TArray<FString> FoundBaseFileNames;
		Algo::Transform(FoundFiles, FoundBaseFileNames, [](const FString& Iterator)
		{
			return FPaths::GetBaseFilename(Iterator);
		});

		for (const FString& FileIt : FoundBaseFileNames)
		{
			ChatSessions.EmplaceAt(FileIt.Equals(NewSessionName.ToString()) ? 0 : ChatSessions.Num(), MakeShared<FName>(FileIt));
		}

		if (FoundBaseFileNames.IsEmpty() || !FoundBaseFileNames.Contains(NewSessionName.ToString()))
		{
			ChatSessions.EmplaceAt(0, MakeShared<FName>(NewSessionName));
		}

		InitializeChatSession(ChatSessions[0]);
	}
	else if (IFileManager::Get().MakeDirectory(*SessionsPath, true))
	{
		InitializeChatSessionOptions();
		return;
	}

	if (ChatSessionListView.IsValid())
	{
		ChatSessionListView->RequestListRefresh();
	}
}

void SWatsonxVRChatShell::InitializeChatSession(const FNamePtr& InItem)
{
	if (!ShellBox.IsValid())
	{
		return;
	}

	ShellBox->SetContent(SAssignNew(CurrentView, SWatsonxVRChatView).SessionID(*InItem));

	if (ChatSessionListView.IsValid())
	{
		if (!ChatSessionListView->IsItemSelected(InItem))
		{
			ChatSessionListView->SetSelection(InItem);
		}

		ChatSessionListView->RequestListRefresh();
	}
}

TSharedRef<ITableRow> SWatsonxVRChatShell::OnGenerateChatSessionRow(FNamePtr InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SWatsonxVRChatSessionOption, OwnerTable, InItem).OnNameChanged(this, &SWatsonxVRChatShell::OnChatSessionNameChanged);
}

void SWatsonxVRChatShell::OnChatSessionSelectionChanged(const FNamePtr InItem, [[maybe_unused]] ESelectInfo::Type SelectInfo)
{
	if (!InItem.IsValid())
	{
		return;
	}

	InitializeChatSession(InItem);
}

void SWatsonxVRChatShell::OnChatSessionNameChanged(const FNamePtr InItem, const FName& NewName)
{
	if (!InItem.IsValid())
	{
		return;
	}

	if (InItem->IsEqual(NewSessionName))
	{
		ChatSessions.EmplaceAt(0, MakeShared<FName>(NewSessionName));
	}

	if (const FString SessionPath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("WatsonxVR"), InItem->ToString()); FPaths::FileExists(SessionPath))
	{
		const FString NewPath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("WatsonxVR"), NewName.ToString());
		IFileManager::Get().Move(*SessionPath, *NewPath, true, true, false, false);
	}

	if (!CurrentView.IsValid())
	{
		return;
	}

	if (CurrentView.IsValid() && CurrentView->GetSessionID().IsEqual(*InItem))
	{
		CurrentView->SetSessionID(NewName);
	}

	*InItem = NewName;

	if (ChatSessionListView.IsValid())
	{
		ChatSessionListView->RequestListRefresh();
	}
}

void SWatsonxVRChatShell::OnChatSessionDoubleClicked(const FNamePtr InItem)
{
	if (!InItem.IsValid() || !ChatSessionListView.IsValid())
	{
		return;
	}

	if (const TSharedPtr<ITableRow> Row = ChatSessionListView->WidgetFromItem(InItem); Row.IsValid())
	{
		if (const TSharedPtr<SWatsonxVRChatSessionOption> Session = StaticCastSharedPtr<SWatsonxVRChatSessionOption>(Row); Session.IsValid())
		{
			Session->EnableEditMode();
			ChatSessionListView->RequestListRefresh();
		}
	}
}

FReply SWatsonxVRChatShell::OnChatSessionKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!ChatSessionListView.IsValid())
	{
		return FReply::Unhandled();
	}

	if (InKeyEvent.GetKey() != EKeys::Delete)
	{
		return FReply::Unhandled();
	}

	if (FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString("Are you sure you want to delete this session?")) == EAppReturnType::No)
	{
		return FReply::Unhandled();
	}

	const FNamePtr SelectedItem = ChatSessionListView->GetNumItemsSelected() == 0 ? nullptr : ChatSessionListView->GetSelectedItems()[0];
	if (!SelectedItem.IsValid())
	{
		return FReply::Unhandled();
	}

	if (CurrentView.IsValid())
	{
		CurrentView->ClearChat();
	}

	if (const FString SessionPath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("WatsonxVR"), SelectedItem->ToString() + TEXT(".json"));
		FPaths::FileExists(SessionPath))
	{
		IFileManager::Get().Delete(*SessionPath, true, true);
	}

	if (SelectedItem->IsEqual(NewSessionName) || !ChatSessions.ContainsByPredicate([](const FNamePtr& Item)
	{
		return Item->IsEqual(NewSessionName);
	}))
	{
		ChatSessions.EmplaceAt(0, MakeShared<FName>(NewSessionName));
	}

	ChatSessions.Remove(SelectedItem);

	if (ChatSessionListView.IsValid())
	{
		ChatSessionListView->RequestListRefresh();
		ChatSessionListView->SetSelection(ChatSessions[0]);
	}

	return FReply::Handled();
}
