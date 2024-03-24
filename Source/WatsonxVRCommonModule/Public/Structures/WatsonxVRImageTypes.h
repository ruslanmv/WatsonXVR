



// These structures are defined in the common module due to being used in both modules, to avoid cyclic dependencies.

#pragma once

#include <CoreMinimal.h>
#include "Structures/WatsonxVRCommonTypes.h"
#include "WatsonxVRImageTypes.generated.h"

UENUM(BlueprintType, Category = "WatsonxVR | Image", Meta = (DisplayName = "WatsonxVR Image Size"))
enum class EWatsonxVRImageSize : uint8
{
	x256 UMETA(DisplayName = "256x256"),
	x512 UMETA(DisplayName = "512x512"),
	x1024 UMETA(DisplayName = "1024x1024"),
};

UENUM(BlueprintType, Category = "WatsonxVR | Image", Meta = (DisplayName = "WatsonxVR Image Response Format"))
enum class EWatsonxVRResponseFormat : uint8
{
	url UMETA(DisplayName = "URL"),
	b64_json UMETA(DisplayName = "B64")};

USTRUCT(BlueprintType, Category = "WatsonxVR | Image", Meta = (DisplayName = "WatsonxVR Image Data"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRImageData
{
	GENERATED_BODY()

	FWatsonxVRImageData() = default;

	FWatsonxVRImageData(const FString& Data, const EWatsonxVRResponseFormat& DataFormat) : Content(Data), Format(DataFormat)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image")
	FString Content;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image")
	EWatsonxVRResponseFormat Format = EWatsonxVRResponseFormat::b64_json;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Image", Meta = (DisplayName = "WatsonxVR Image Response"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRImageResponse
{
	GENERATED_BODY()

	FWatsonxVRImageResponse() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image")
	int32 Created = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image")
	TArray<FWatsonxVRImageData> Data;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image")
	bool bSuccess = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image")
	FWatsonxVRCommonError Error;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Image", Meta = (DisplayName = "WatsonxVR Image Options"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRImageOptions
{
	GENERATED_BODY()

	FWatsonxVRImageOptions();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image",
		Meta = (DisplayName = "Number of Images", ClampMin = "1", UIMin = "1", ClampMax = "10", UIMax = "10"))
	int32 ImagesNum;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image", Meta = (DisplayName = "Image Size"))
	EWatsonxVRImageSize Size = EWatsonxVRImageSize::x256;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Image", Meta = (DisplayName = "Response Format"))
	EWatsonxVRResponseFormat Format = EWatsonxVRResponseFormat::b64_json;

private:
	void SetDefaults();
};
