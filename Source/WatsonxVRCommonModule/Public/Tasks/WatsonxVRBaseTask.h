



#pragma once

#include <CoreMinimal.h>
#include <Interfaces/IHttpRequest.h>
#include <Kismet/BlueprintAsyncActionBase.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "Structures/WatsonxVRCommonTypes.h"
#include "WatsonxVRBaseTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWatsonxVRGenericDelegate);

/**
 *
 */
UCLASS(NotPlaceable, Category = "WatsonxVR", meta = (ExposedAsyncProxy = AsyncTask))
class WatsonxVRCOMMONMODULE_API UWatsonxVRBaseTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UWatsonxVRTaskStatus;

public:
	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR")
	FWatsonxVRGenericDelegate RequestFailed;

	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR")
	FWatsonxVRGenericDelegate RequestSent;

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR", meta = (DisplayName = "Stop WatsonxVR Task"))
	void StopWatsonxVRTask();

	virtual void SetReadyToDestroy() override;

	UFUNCTION(BlueprintPure, Category = "WatsonxVR", Meta = (DisplayName = "Get API Key"))
	const FWatsonxVRCommonOptions GetCommonOptions() const;

protected:
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest;
	FWatsonxVRCommonOptions CommonOptions;

	mutable FCriticalSection Mutex;

	virtual bool CanActivateTask() const;
	virtual bool CanBindProgress() const;
	virtual FString GetEndpointURL() const;

	void SendRequest();

	/* Return true if contains error */
	const bool CheckError(const TSharedPtr<class FJsonObject>& JsonObject, FWatsonxVRCommonError& OutputError) const;

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

UCLASS(NotPlaceable, Category = "WatsonxVR")
class WatsonxVRCOMMONMODULE_API UWatsonxVRTaskStatus final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "WatsonxVR")
	static bool IsTaskActive(const UWatsonxVRBaseTask* Test);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR")
	static bool IsTaskReadyToDestroy(const UWatsonxVRBaseTask* Test);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR")
	static bool IsTaskStillValid(const UWatsonxVRBaseTask* Test);
};
