



#pragma once

#include <CoreMinimal.h>
#include "AppCommonTypes.generated.h"

USTRUCT(BlueprintType, Category = "App | Common", Meta = (DisplayName = "App Common Error"))
struct AppCOMMONMODULE_API FAppCommonError
{
	GENERATED_BODY()

	FAppCommonError() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common")
	FName Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common")
	FName Code;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common")
	FString Message;
};

USTRUCT(BlueprintType, Category = "App | Common", Meta = (DisplayName = "App Common Options"))
struct AppCOMMONMODULE_API FAppCommonOptions
{
	GENERATED_BODY()

	FAppCommonOptions();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common", Meta = (DisplayName = "API Key"))
	FName APIKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common", Meta = (DisplayName = "User"))
	FName User;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common", Meta = (DisplayName = "Is Azure OpenAI"))
	bool bIsAzureOpenAI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common", Meta = (DisplayName = "Endpoint", EditCondition = "bIsAzureOpenAI"))
	FString Endpoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Common",
		Meta = (DisplayName = "Azure OpenAI API Version", EditCondition = "bIsAzureOpenAI"))
	FString AzureOpenAIAPIVersion;

private:
	void SetDefaults();
};
