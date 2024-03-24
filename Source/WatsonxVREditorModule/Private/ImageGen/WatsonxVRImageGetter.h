



#pragma once

#include <CoreMinimal.h>
#include <Engine/Texture2D.h>
#include <Tasks/WatsonxVRImageRequest.h>
#include "WatsonxVRImageGetter.generated.h"

DECLARE_DELEGATE_OneParam(FImageGenerated, UTexture2D*);
DECLARE_DELEGATE_OneParam(FImageStatusChanged, FString);

UCLASS(MinimalAPI, NotBlueprintable, NotPlaceable, Category = "Implementation")
class UWatsonxVRImageGetter : public UObject
{
	GENERATED_BODY()

public:
	explicit UWatsonxVRImageGetter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FImageGenerated OnImageGenerated;
	FImageStatusChanged OnStatusChanged;

	UFUNCTION()
	void RequestSent();

	UFUNCTION()
	void RequestFailed();

	UFUNCTION()
	void ProcessCompleted(const FWatsonxVRImageResponse& Response);

	void Destroy();

	TSharedPtr<class SScrollBox> OutScrollBox;

private:
	void ProcessImage(const FWatsonxVRImageData& Data);

	FWatsonxVRImageGenerate OnImageGenerated_Internal;

	UFUNCTION()
	void ImageGenerated(UTexture2D* const Texture);

	uint8 GeneratedImages;
	uint8 DataSize;
};
