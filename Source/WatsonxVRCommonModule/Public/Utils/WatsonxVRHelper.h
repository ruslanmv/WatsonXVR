



#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "Structures/WatsonxVRChatTypes.h"
#include "Structures/WatsonxVRImageTypes.h"
#include "WatsonxVRHelper.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "WatsonxVR")
class WatsonxVRCOMMONMODULE_API UWatsonxVRHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Convert WatsonxVR Model to Name"))
	static const FName ModelToName(const EWatsonxVRChatModel Model);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Convert Name to WatsonxVR Model"))
	static const EWatsonxVRChatModel NameToModel(const FName Model);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Convert WatsonxVR Role to Name"))
	static const FName RoleToName(const EWatsonxVRChatRole Role);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Convert Name to WatsonxVR Role"))
	static const EWatsonxVRChatRole NameToRole(const FName Role);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Convert WatsonxVR Param Type to Name"))
	static const FName PropertyTypeToName(const EWatsonxVRPropertyType Type);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Convert Name to WatsonxVR Param Type"))
	static const EWatsonxVRPropertyType NameToPropertyType(const FName Type);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Get Available GPT Models"))
	static const TArray<FName> GetAvailableGPTModels();

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Get Endpoint for Model"))
	static const FString GetEndpointForModel(const EWatsonxVRChatModel Model, const bool bIsAzureOpenAI = false,
	                                         const FString& AzureOpenAIAPIVersion = TEXT("None"));

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", meta = (DisplayName = "Model Supports Chat"))
	static const bool ModelSupportsChat(const EWatsonxVRChatModel Model);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Image", meta = (DisplayName = "Convert WatsonxVR Size to Name"))
	static const FName SizeToName(const EWatsonxVRImageSize Size);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Image", meta = (DisplayName = "Convert Name to WatsonxVR Size"))
	static const EWatsonxVRImageSize NameToSize(const FName Size);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Image", meta = (DisplayName = "Convert WatsonxVR Format to Name"))
	static const FName FormatToName(const EWatsonxVRResponseFormat Format);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Image", meta = (DisplayName = "Convert Name to WatsonxVR Format"))
	static const EWatsonxVRResponseFormat NameToFormat(const FName Format);
};
