



#pragma once

#include <CoreMinimal.h>
#include <Engine/Texture2D.h>
#include <Tasks/AppImageRequest.h>
#include "AppImageGetter.generated.h"

DECLARE_DELEGATE_OneParam(FImageGenerated, UTexture2D*);
DECLARE_DELEGATE_OneParam(FImageStatusChanged, FString);

UCLASS(MinimalAPI, NotBlueprintable, NotPlaceable, Category = "Implementation")
class UAppImageGetter : public UObject
{
	GENERATED_BODY()

public:
	explicit UAppImageGetter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FImageGenerated OnImageGenerated;
	FImageStatusChanged OnStatusChanged;

	UFUNCTION()
	void RequestSent();

	UFUNCTION()
	void RequestFailed();

	UFUNCTION()
	void ProcessCompleted(const FAppImageResponse& Response);

	void Destroy();

	TSharedPtr<class SScrollBox> OutScrollBox;

private:
	void ProcessImage(const FAppImageData& Data);

	FAppImageGenerate OnImageGenerated_Internal;

	UFUNCTION()
	void ImageGenerated(UTexture2D* const Texture);

	uint8 GeneratedImages;
	uint8 DataSize;
};
