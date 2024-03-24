



#pragma once

#include <CoreMinimal.h>

class FWatsonxVREditorModule : public IModuleInterface
{
protected:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs) const;

private:
	void RegisterMenus();
	FPropertyEditorModule* PropertyEditorModule = nullptr;
};
