



#pragma once

#include <CoreMinimal.h>
#include <Interfaces/IHttpRequest.h>
#include <Kismet/BlueprintAsyncActionBase.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "Structures/AppCommonTypes.h"
#include "AppBaseTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAppGenericDelegate);

/**
 *
 */
UCLASS(NotPlaceable, Category = "App", meta = (ExposedAsyncProxy = AsyncTask))
class AppCOMMONMODULE_API UAppBaseTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UAppTaskStatus;

public:
	UPROPERTY(BlueprintAssignable, Category = "App")
	FAppGenericDelegate RequestFailed;

	UPROPERTY(BlueprintAssignable, Category = "App")
	FAppGenericDelegate RequestSent;

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "App", meta = (DisplayName = "Stop App Task"))
	void StopAppTask();

	virtual void SetReadyToDestroy() override;

	UFUNCTION(BlueprintPure, Category = "App", Meta = (DisplayName = "Get API Key"))
	const FAppCommonOptions GetCommonOptions() const;

protected:
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest;
	FAppCommonOptions CommonOptions;

	mutable FCriticalSection Mutex;

	virtual bool CanActivateTask() const;
	virtual bool CanBindProgress() const;
	virtual FString GetEndpointURL() const;

	void SendRequest();

	/* Return true if contains error */
	const bool CheckError(const TSharedPtr<class FJsonObject>& JsonObject, FAppCommonError& OutputError) const;

	void InitializeRequest();
	void BindRequestCallbacks();

	virtual FString SetRequestContent() { return FString(); };

	virtual void OnProgressUpdated(const FString& Content, int32 BytesSent, int32 BytesReceived)
	{
	};

	virtual void OnProgressCompleted(const FString& Content, const bool bWasSuccessful)
	{
	};

	bool bInitialized = false;
	bool bIsReadyToDestroy = false;
	bool bIsTaskActive = false;

#if WITH_EDITOR
	bool bIsEditorTask = false;
	bool bEndingPIE = false;

	virtual void PrePIEEnded(bool bIsSimulating);
#endif
};

UCLASS(NotPlaceable, Category = "App")
class AppCOMMONMODULE_API UAppTaskStatus final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "App")
	static bool IsTaskActive(const UAppBaseTask* Test);

	UFUNCTION(BlueprintPure, Category = "App")
	static bool IsTaskReadyToDestroy(const UAppBaseTask* Test);

	UFUNCTION(BlueprintPure, Category = "App")
	static bool IsTaskStillValid(const UAppBaseTask* Test);
};
