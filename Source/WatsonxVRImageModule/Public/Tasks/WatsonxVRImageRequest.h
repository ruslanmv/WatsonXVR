



#pragma once





#pragma once

#include <CoreMinimal.h>
#include <Tasks/WatsonxVRBaseTask.h>
#include <Structures/WatsonxVRCommonTypes.h>
#include <Structures/WatsonxVRImageTypes.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "WatsonxVRImageRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWatsonxVRImageResponseDelegate, const FWatsonxVRImageResponse&, Response);

/**
 *
 */
UCLASS(NotPlaceable, Category = "WatsonxVR | Image", meta = (ExposedAsyncProxy = AsyncTask))
class WatsonxVRIMAGEMODULE_API UWatsonxVRImageRequest : public UWatsonxVRBaseTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Image")
	FWatsonxVRImageResponseDelegate ProcessCompleted;

	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Image")
	FWatsonxVRImageResponseDelegate ProgressUpdated;

	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Image")
	FWatsonxVRImageResponseDelegate ProgressStarted;

	UPROPERTY(BlueprintAssignable, Category = "WatsonxVR | Image")
	FWatsonxVRImageResponseDelegate ErrorReceived;

#if WITH_EDITOR
	static UWatsonxVRImageRequest* EditorTask(const FString& Prompt, const FWatsonxVRImageOptions Options);
#endif

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR | Image | Default",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Request Images with Default Options"))
	static UWatsonxVRImageRequest* RequestImages_DefaultOptions(UObject* const WorldContextObject, const FString& Prompt);

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR | Image | Custom",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Request Images with Custom Options"))
	static UWatsonxVRImageRequest* RequestImages_CustomOptions(UObject* const WorldContextObject, const FString& Prompt,
	                                                         const FWatsonxVRCommonOptions CommonOptions, const FWatsonxVRImageOptions ImageOptions);

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Image")
	const FWatsonxVRImageOptions GetImageOptions() const;

	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Image")
	const FString GetPrompt() const;

protected:
	FString Prompt;
	FWatsonxVRImageOptions ImageOptions;

	virtual bool CanActivateTask() const override;
	virtual bool CanBindProgress() const override;
	virtual FString GetEndpointURL() const override;

	virtual FString SetRequestContent() override;
	virtual void OnProgressCompleted(const FString& Content, const bool bWasSuccessful) override;

	void DeserializeResponse(const FString& Content);

private:
	FWatsonxVRImageResponse Response;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FWatsonxVRImageGenerate, class UTexture2D*, Image);

UCLASS(NotPlaceable, Category = "WatsonxVR | Image", Meta = (DisplayName = "WatsonxVR Image Helper"))
class WatsonxVRIMAGEMODULE_API UWatsonxVRImageHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "WatsonxVR | Image", Meta = (DisplayName = "Cast to WatsonxVR Image Request"))
	static UWatsonxVRImageRequest* CastToWatsonxVRImageRequest(UObject* const Object);

	UFUNCTION(BlueprintCallable, Category = "WatsonxVR | Image")
	static void GenerateImage(const FWatsonxVRImageData& ImageData, const FWatsonxVRImageGenerate& Callback);

private:
	static void GenerateImageFromURL(const FWatsonxVRImageData& ImageData, const FWatsonxVRImageGenerate& Callback);
	static void GenerateImageFromB64(const FWatsonxVRImageData& ImageData, const FWatsonxVRImageGenerate& Callback);
};
