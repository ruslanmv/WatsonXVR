



#pragma once

#include <CoreMinimal.h>
#include <Tasks/AppBaseTask.h>
#include <Structures/AppCommonTypes.h>
#include <Structures/AppChatTypes.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "AppChatRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAppChatResponseDelegate, const FAppChatResponse&, Response);

/**
 *
 */
UCLASS(NotPlaceable, Category = "App | Chat", meta = (ExposedAsyncProxy = AsyncTask))
class AppCHATMODULE_API UAppChatRequest : public UAppBaseTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "App | Chat")
	FAppChatResponseDelegate ProcessCompleted;

	UPROPERTY(BlueprintAssignable, Category = "App | Chat")
	FAppChatResponseDelegate ProgressUpdated;

	UPROPERTY(BlueprintAssignable, Category = "App | Chat")
	FAppChatResponseDelegate ProgressStarted;

	UPROPERTY(BlueprintAssignable, Category = "App | Chat")
	FAppChatResponseDelegate ErrorReceived;

#if WITH_EDITOR
	static UAppChatRequest* EditorTask(const TArray<FAppChatMessage>& Messages, const FAppChatOptions& Options);
#endif

	UFUNCTION(BlueprintCallable, Category = "App | Chat | Default",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Message with Default Options",
			AutoCreateRefTerm = "Functions"))
	static UAppChatRequest* SendMessage_DefaultOptions(UObject* const WorldContextObject, const FString& Message,
	                                                       const TArray<FAppFunction>& Functions);

	UFUNCTION(BlueprintCallable, Category = "App | Chat | Default",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Messages with Default Options",
			AutoCreateRefTerm = "Functions"))
	static UAppChatRequest* SendMessages_DefaultOptions(UObject* const WorldContextObject, const TArray<FAppChatMessage>& Messages,
	                                                        const TArray<FAppFunction>& Functions);

	UFUNCTION(BlueprintCallable, Category = "App | Chat | Custom",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Message with Custom Options",
			AutoCreateRefTerm = "Functions"))
	static UAppChatRequest* SendMessage_CustomOptions(UObject* const WorldContextObject, const FString& Message,
	                                                      const TArray<FAppFunction>& Functions, const FAppCommonOptions CommonOptions,
	                                                      const FAppChatOptions ChatOptions);

	UFUNCTION(BlueprintCallable, Category = "App | Chat | Custom",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Messages with Custom Options",
			AutoCreateRefTerm = "Functions"))
	static UAppChatRequest* SendMessages_CustomOptions(UObject* const WorldContextObject, const TArray<FAppChatMessage>& Messages,
	                                                       const TArray<FAppFunction>& Functions, const FAppCommonOptions CommonOptions,
	                                                       const FAppChatOptions ChatOptions);

	UFUNCTION(BlueprintPure, Category = "App | Chat")
	const FAppChatOptions GetChatOptions() const;

protected:
	TArray<FAppChatMessage> Messages;
	TArray<FAppFunction> Functions;
	FAppChatOptions ChatOptions;

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
	FAppChatResponse Response;
};

UCLASS(NotPlaceable, Category = "App | Chat", Meta = (DisplayName = "App Chat Helper"))
class AppCHATMODULE_API UAppChatHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "App | Chat", Meta = (DisplayName = "Cast to App Chat Request"))
	static UAppChatRequest* CastToAppChatRequest(UObject* const Object);
};
