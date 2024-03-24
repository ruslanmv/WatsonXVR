



#pragma once

#include <CoreMinimal.h>
#include "WatsonxVRCommonTypes.generated.h"

USTRUCT(BlueprintType, Category = "WatsonxVR | Common", Meta = (DisplayName = "WatsonxVR Common Error"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRCommonError
{
	GENERATED_BODY()

	FWatsonxVRCommonError() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common")
	FName Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common")
	FName Code;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common")
	FString Message;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Common", Meta = (DisplayName = "WatsonxVR Common Options"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRCommonOptions
{
	GENERATED_BODY()

	FWatsonxVRCommonOptions();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common", Meta = (DisplayName = "API Key"))
	FName APIKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common", Meta = (DisplayName = "User"))
	FName User;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common", Meta = (DisplayName = "Is Azure OpenAI"))
	bool bIsAzureOpenAI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common", Meta = (DisplayName = "Endpoint", EditCondition = "bIsAzureOpenAI"))
	FString Endpoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Common",
		Meta = (DisplayName = "Azure OpenAI API Version", EditCondition = "bIsAzureOpenAI"))
	FString AzureOpenAIAPIVersion;

private:
	void SetDefaults();
};
