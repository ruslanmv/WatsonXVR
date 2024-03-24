



#pragma once

#include <CoreMinimal.h>
#include <Tasks/WatsonxVRBaseTask.h>
#include <Structures/WatsonxVRCommonTypes.h>
#include <Structures/WatsonxVRChatTypes.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "WatsonxVRChatRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWatsonxVRChatResponseDelegate, const FWatsonxVRChatResponse&, Response);

/**
 *
 */
UCLASS(NotPlaceable, Category = "WatsonxVR | Chat", meta = (ExposedAsyncProxy = AsyncTask))
class WatsonxVRCHATMODULE_API UWatsonxVRChatRequest : public UWatsonxVRBaseTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Chat")
	FWatsonxVRChatResponseDelegate ProcessCompleted;

	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Chat")
	FWatsonxVRChatResponseDelegate ProgressUpdated;

	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Chat")
	FWatsonxVRChatResponseDelegate ProgressStarted;

	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Chat")
	FWatsonxVRChatResponseDelegate ErrorReceived;

#if WITH_EDITOR
	static UWatsonxVRChatRequest* EditorTask(const TArray<FWatsonxVRChatMessage>& Messages, const FWatsonxVRChatOptions& Options);
#endif

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR | Chat | Default",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Message with Default Options",
			AutoCreateRefTerm = "Functions"))
	static UWatsonxVRChatRequest* SendMessage_DefaultOptions(UObject* const WorldContextObject, const FString& Message,
	                                                       const TArray<FWatsonxVRFunction>& Functions);

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR | Chat | Default",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Messages with Default Options",
			AutoCreateRefTerm = "Functions"))
	static UWatsonxVRChatRequest* SendMessages_DefaultOptions(UObject* const WorldContextObject, const TArray<FWatsonxVRChatMessage>& Messages,
	                                                        const TArray<FWatsonxVRFunction>& Functions);

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR | Chat | Custom",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Message with Custom Options",
			AutoCreateRefTerm = "Functions"))
	static UWatsonxVRChatRequest* SendMessage_CustomOptions(UObject* const WorldContextObject, const FString& Message,
	                                                      const TArray<FWatsonxVRFunction>& Functions, const FWatsonxVRCommonOptions CommonOptions,
	                                                      const FWatsonxVRChatOptions ChatOptions);

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR | Chat | Custom",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Messages with Custom Options",
			AutoCreateRefTerm = "Functions"))
	static UWatsonxVRChatRequest* SendMessages_CustomOptions(UObject* const WorldContextObject, const TArray<FWatsonxVRChatMessage>& Messages,
	                                                       const TArray<FWatsonxVRFunction>& Functions, const FWatsonxVRCommonOptions CommonOptions,
	                                                       const FWatsonxVRChatOptions ChatOptions);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat")
	const FWatsonxVRChatOptions GetChatOptions() const;

protected:
	TArray<FWatsonxVRChatMessage> Messages;
	TArray<FWatsonxVRFunction> Functions;
	FWatsonxVRChatOptions ChatOptions;

	virtual bool CanActivateTask() const override;
	virtual bool CanBindProgress() const override;
	virtual FString GetEndpointURL() const override;

	virtual FString SetRequestContent() override;
	virtual void OnProgressUpdated(const FString& Content, int32 BytesSent, int32 BytesReceived) override;
	virtual void OnProgressCompleted(const FString& Content, const bool bWasSuccessful) override;

	TArray<FString> GetDeltasFromContent(const FString& Content) const;

	void DeserializeStreamedResponse(const TArray<FString>& Deltas);
	void DeserializeSingleResponse(const FString& Content);

private:
	FWatsonxVRChatResponse Response;
};

UCLASS(NotPlaceable, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Helper"))
class WatsonxVRCHATMODULE_API UWatsonxVRChatHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Cast to WatsonxVR Chat Request"))
	static UWatsonxVRChatRequest* CastToWatsonxVRChatRequest(UObject* const Object);
};
