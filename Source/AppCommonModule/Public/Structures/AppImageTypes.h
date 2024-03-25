



// These structures are defined in the common module due to being used in both modules, to avoid cyclic dependencies.

#pragma once

#include <CoreMinimal.h>
#include "Structures/AppCommonTypes.h"
#include "AppImageTypes.generated.h"

UENUM(BlueprintType, Category = "App | Image", Meta = (DisplayName = "App Image Size"))
enum class EAppImageSize : uint8
{
	x256 UMETA(DisplayName = "256x256"),
	x512 UMETA(DisplayName = "512x512"),
	x1024 UMETA(DisplayName = "1024x1024"),
};

UENUM(BlueprintType, Category = "App | Image", Meta = (DisplayName = "App Image Response Format"))
enum class EAppResponseFormat : uint8
{
	url UMETA(DisplayName = "URL"),
	b64_json UMETA(DisplayName = "B64")};

USTRUCT(BlueprintType, Category = "App | Image", Meta = (DisplayName = "App Image Data"))
struct AppCOMMONMODULE_API FAppImageData
{
	GENERATED_BODY()

	FAppImageData() = default;

	FAppImageData(const FString& Data, const EAppResponseFormat& DataFormat) : Content(Data), Format(DataFormat)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image")
	FString Content;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image")
	EAppResponseFormat Format = EAppResponseFormat::b64_json;
};

USTRUCT(BlueprintType, Category = "App | Image", Meta = (DisplayName = "App Image Response"))
struct AppCOMMONMODULE_API FAppImageResponse
{
	GENERATED_BODY()

	FAppImageResponse() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image")
	int32 Created = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image")
	TArray<FAppImageData> Data;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image")
	bool bSuccess = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image")
	FAppCommonError Error;
};

USTRUCT(BlueprintType, Category = "App | Image", Meta = (DisplayName = "App Image Options"))
struct AppCOMMONMODULE_API FAppImageOptions
{
	GENERATED_BODY()

	FAppImageOptions();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image",
		Meta = (DisplayName = "Number of Images", ClampMin = "1", UIMin = "1", ClampMax = "10", UIMax = "10"))
	int32 ImagesNum;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image", Meta = (DisplayName = "Image Size"))
	EAppImageSize Size = EAppImageSize::x256;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Image", Meta = (DisplayName = "Response Format"))
	EAppResponseFormat Format = EAppResponseFormat::b64_json;

private:
	void SetDefaults();
};
