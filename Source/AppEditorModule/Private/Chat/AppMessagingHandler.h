

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>
#include <Structures/AppChatTypes.h>
#include "AppMessagingHandler.generated.h"

DECLARE_DELEGATE_OneParam(FMessageContentUpdated, FString);

UCLASS(MinimalAPI, NotBlueprintable, NotPlaceable, Category = "Implementation")
class UAppMessagingHandler : public UObject
{
	GENERATED_BODY()

public:
	explicit UAppMessagingHandler(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FMessageContentUpdated OnMessageContentUpdated;

	UFUNCTION()
	void RequestSent();

	UFUNCTION()
	void RequestFailed();

	UFUNCTION()
	void ProcessUpdated(const FAppChatResponse& Response);

	UFUNCTION()
	void ProcessCompleted(const FAppChatResponse& Response);

	TSharedPtr<class SScrollBox> ScrollBoxReference;

	void Destroy();

private:
	void ProcessResponse(const FAppChatResponse& Response);
};
