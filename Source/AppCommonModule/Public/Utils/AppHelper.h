



#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "Structures/AppChatTypes.h"
#include "Structures/AppImageTypes.h"
#include "AppHelper.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "App")
class AppCOMMONMODULE_API UAppHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Convert App Model to Name"))
	static const FName ModelToName(const EAppChatModel Model);

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Convert Name to App Model"))
	static const EAppChatModel NameToModel(const FName Model);

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Convert App Role to Name"))
	static const FName RoleToName(const EAppChatRole Role);

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Convert Name to App Role"))
	static const EAppChatRole NameToRole(const FName Role);

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Convert App Param Type to Name"))
	static const FName PropertyTypeToName(const EAppPropertyType Type);

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Convert Name to App Param Type"))
	static const EAppPropertyType NameToPropertyType(const FName Type);

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Get Available GPT Models"))
	static const TArray<FName> GetAvailableGPTModels();

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Get Endpoint for Model"))
	static const FString GetEndpointForModel(const EAppChatModel Model, const bool bIsAzureOpenAI = false,
	                                         const FString& AzureOpenAIAPIVersion = TEXT("None"));

	UFUNCTION(BlueprintPure, Category = "App | Chat", meta = (DisplayName = "Model Supports Chat"))
	static const bool ModelSupportsChat(const EAppChatModel Model);

	UFUNCTION(BlueprintPure, Category = "App | Image", meta = (DisplayName = "Convert App Size to Name"))
	static const FName SizeToName(const EAppImageSize Size);

	UFUNCTION(BlueprintPure, Category = "App | Image", meta = (DisplayName = "Convert Name to App Size"))
	static const EAppImageSize NameToSize(const FName Size);

	UFUNCTION(BlueprintPure, Category = "App | Image", meta = (DisplayName = "Convert App Format to Name"))
	static const FName FormatToName(const EAppResponseFormat Format);

	UFUNCTION(BlueprintPure, Category = "App | Image", meta = (DisplayName = "Convert Name to App Format"))
	static const EAppResponseFormat NameToFormat(const FName Format);
};
