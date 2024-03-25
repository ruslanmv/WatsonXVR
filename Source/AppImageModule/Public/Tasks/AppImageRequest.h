



#pragma once





#pragma once

#include <CoreMinimal.h>
#include <Tasks/AppBaseTask.h>
#include <Structures/AppCommonTypes.h>
#include <Structures/AppImageTypes.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "AppImageRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAppImageResponseDelegate, const FAppImageResponse&, Response);

/**
 *
 */
UCLASS(NotPlaceable, Category = "App | Image", meta = (ExposedAsyncProxy = AsyncTask))
class AppIMAGEMODULE_API UAppImageRequest : public UAppBaseTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "App | Image")
	FAppImageResponseDelegate ProcessCompleted;

	UPROPERTY(BlueprintAssignable, Category = "App | Image")
	FAppImageResponseDelegate ProgressUpdated;

	UPROPERTY(BlueprintAssignable, Category = "App | Image")
	FAppImageResponseDelegate ProgressStarted;

	UPROPERTY(BlueprintAssignable, Category = "App | Image")
	FAppImageResponseDelegate ErrorReceived;

#if WITH_EDITOR
	static UAppImageRequest* EditorTask(const FString& Prompt, const FAppImageOptions Options);
#endif

	UFUNCTION(BlueprintCallable, Category = "App | Image | Default",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Request Images with Default Options"))
	static UAppImageRequest* RequestImages_DefaultOptions(UObject* const WorldContextObject, const FString& Prompt);

	UFUNCTION(BlueprintCallable, Category = "App | Image | Custom",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Request Images with Custom Options"))
	static UAppImageRequest* RequestImages_CustomOptions(UObject* const WorldContextObject, const FString& Prompt,
	                                                         const FAppCommonOptions CommonOptions, const FAppImageOptions ImageOptions);

	UFUNCTION(BlueprintPure, Category = "App | Image")
	const FAppImageOptions GetImageOptions() const;

	UFUNCTION(BlueprintPure, Category = "App | Image")
	const FString GetPrompt() const;

protected:
	FString Prompt;
	FAppImageOptions ImageOptions;

	virtual bool CanActivateTask() const override;
	virtual bool CanBindProgress() const override;
	virtual FString GetEndpointURL() const override;

	virtual FString SetRequestContent() override;
	virtual void OnProgressCompleted(const FString& Content, const bool bWasSuccessful) override;

	void DeserializeResponse(const FString& Content);

private:
	FAppImageResponse Response;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FAppImageGenerate, class UTexture2D*, Image);

UCLASS(NotPlaceable, Category = "App | Image", Meta = (DisplayName = "App Image Helper"))
class AppIMAGEMODULE_API UAppImageHelper final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "App | Image", Meta = (DisplayName = "Cast to App Image Request"))
	static UAppImageRequest* CastToAppImageRequest(UObject* const Object);

	UFUNCTION(BlueprintCallable, Category = "App | Image")
	static void GenerateImage(const FAppImageData& ImageData, const FAppImageGenerate& Callback);

private:
	static void GenerateImageFromURL(const FAppImageData& ImageData, const FAppImageGenerate& Callback);
	static void GenerateImageFromB64(const FAppImageData& ImageData, const FAppImageGenerate& Callback);
};
