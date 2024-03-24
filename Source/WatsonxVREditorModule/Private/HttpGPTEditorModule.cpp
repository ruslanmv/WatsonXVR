



#include "WatsonxVREditorModule.h"
#include "Chat/SWatsonxVRChatShell.h"
#include "ImageGen/SWatsonxVRImageGenView.h"
#include <ToolMenus.h>
#include <Widgets/Docking/SDockTab.h>
#include <WorkspaceMenuStructure.h>
#include <WorkspaceMenuStructureModule.h>

static const FName WatsonxVRChatTabName("WatsonxVRChat");
static const FName WatsonxVRImageGeneratorTabName("WatsonxVRImageGenerator");

#define LOCTEXT_NAMESPACE "FWatsonxVREditorModule"

void FWatsonxVREditorModule::StartupModule()
{
	const FSimpleDelegate RegisterDelegate = FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FWatsonxVREditorModule::RegisterMenus);
	UToolMenus::RegisterStartupCallback(RegisterDelegate);
}

void FWatsonxVREditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(WatsonxVRChatTabName);
}

TSharedRef<SDockTab> FWatsonxVREditorModule::OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs) const
{
	const FName TabId = *SpawnTabArgs.GetTabId().ToString();

	TSharedPtr<SWidget> OutContent;
	if (TabId.IsEqual(WatsonxVRChatTabName))
	{
		OutContent = SNew(SWatsonxVRChatShell);
	}
	else if (TabId.IsEqual(WatsonxVRImageGeneratorTabName))
	{
		OutContent = SNew(SWatsonxVRImageGenView);
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

void FWatsonxVREditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	const FOnSpawnTab EditorTabSpawnerDelegate = FOnSpawnTab::CreateRaw(this, &FWatsonxVREditorModule::OnSpawnTab);

#if ENGINE_MAJOR_VERSION < 5
    const FName AppStyleName = FEditorStyle::GetStyleSetName();
#else
	const FName AppStyleName = FAppStyle::GetAppStyleSetName();
#endif

	const TSharedRef<FWorkspaceItem> Menu = WorkspaceMenu::GetMenuStructure().GetToolsCategory()->AddGroup(
		LOCTEXT("WatsonxVRCategory", "WatsonxVR"), LOCTEXT("WatsonxVRCategoryTooltip", "WatsonxVR Plugin Tabs"),
		FSlateIcon(AppStyleName, "Icons.Documentation"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(WatsonxVRChatTabName, EditorTabSpawnerDelegate).
	                          SetDisplayName(FText::FromString(TEXT("WatsonxVR Chat"))).SetTooltipText(FText::FromString(TEXT("Open WatsonxVR Chat"))).
	                          SetIcon(FSlateIcon(AppStyleName, "DerivedData.ResourceUsage")).SetGroup(Menu);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(WatsonxVRImageGeneratorTabName, EditorTabSpawnerDelegate).
	                          SetDisplayName(FText::FromString(TEXT("WatsonxVR Image Generator"))).SetTooltipText(
		                          FText::FromString(TEXT("Open WatsonxVR Image Generator"))).SetIcon(
		                          FSlateIcon(AppStyleName, "LevelEditor.Tabs.Viewports")).SetGroup(Menu);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FWatsonxVREditorModule, WatsonxVREditor)
