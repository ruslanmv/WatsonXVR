



#include "AppEditorModule.h"
#include "Chat/SAppChatShell.h"
#include "ImageGen/SAppImageGenView.h"
#include <ToolMenus.h>
#include <Widgets/Docking/SDockTab.h>
#include <WorkspaceMenuStructure.h>
#include <WorkspaceMenuStructureModule.h>

static const FName AppChatTabName("AppChat");
static const FName AppImageGeneratorTabName("AppImageGenerator");

#define LOCTEXT_NAMESPACE "FAppEditorModule"

void FAppEditorModule::StartupModule()
{
	const FSimpleDelegate RegisterDelegate = FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAppEditorModule::RegisterMenus);
	UToolMenus::RegisterStartupCallback(RegisterDelegate);
}

void FAppEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AppChatTabName);
}

TSharedRef<SDockTab> FAppEditorModule::OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs) const
{
	const FName TabId = *SpawnTabArgs.GetTabId().ToString();

	TSharedPtr<SWidget> OutContent;
	if (TabId.IsEqual(AppChatTabName))
	{
		OutContent = SNew(SAppChatShell);
	}
	else if (TabId.IsEqual(AppImageGeneratorTabName))
	{
		OutContent = SNew(SAppImageGenView);
	}

	if (OutContent.IsValid())
	{
		return SNew(SDockTab).TabRole(NomadTab)
			[
				OutContent.ToSharedRef()
			];
	}

	return SNew(SDockTab);
}

void FAppEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	const FOnSpawnTab EditorTabSpawnerDelegate = FOnSpawnTab::CreateRaw(this, &FAppEditorModule::OnSpawnTab);

#if ENGINE_MAJOR_VERSION < 5
    const FName AppStyleName = FEditorStyle::GetStyleSetName();
#else
	const FName AppStyleName = FAppStyle::GetAppStyleSetName();
#endif

	const TSharedRef<FWorkspaceItem> Menu = WorkspaceMenu::GetMenuStructure().GetToolsCategory()->AddGroup(
		LOCTEXT("AppCategory", "App"), LOCTEXT("AppCategoryTooltip", "App Plugin Tabs"),
		FSlateIcon(AppStyleName, "Icons.Documentation"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AppChatTabName, EditorTabSpawnerDelegate).
	                          SetDisplayName(FText::FromString(TEXT("App Chat"))).SetTooltipText(FText::FromString(TEXT("Open App Chat"))).
	                          SetIcon(FSlateIcon(AppStyleName, "DerivedData.ResourceUsage")).SetGroup(Menu);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AppImageGeneratorTabName, EditorTabSpawnerDelegate).
	                          SetDisplayName(FText::FromString(TEXT("App Image Generator"))).SetTooltipText(
		                          FText::FromString(TEXT("Open App Image Generator"))).SetIcon(
		                          FSlateIcon(AppStyleName, "LevelEditor.Tabs.Viewports")).SetGroup(Menu);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAppEditorModule, AppEditor)
