

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>
#include <Structures/WatsonxVRChatTypes.h>
#include "WatsonxVRMessagingHandler.generated.h"

DECLARE_DELEGATE_OneParam(FMessageContentUpdated, FString);

UCLASS(MinimalAPI, NotBlueprintable, NotPlaceable, Category = "Implementation")
class UWatsonxVRMessagingHandler : public UObject
{
	GENERATED_BODY()

public:
	explicit UWatsonxVRMessagingHandler(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FMessageContentUpdated OnMessageContentUpdated;

	UFUNCTION()
	void RequestSent();

	UFUNCTION()
	void RequestFailed();

	UFUNCTION()
	void ProcessUpdated(const FWatsonxVRChatResponse& Response);

	UFUNCTION()
	void ProcessCompleted(const FWatsonxVRChatResponse& Response);

	TSharedPtr<class SScrollBox> ScrollBoxReference;

	void Destroy();

private:
	void ProcessResponse(const FWatsonxVRChatResponse& Response);
};
