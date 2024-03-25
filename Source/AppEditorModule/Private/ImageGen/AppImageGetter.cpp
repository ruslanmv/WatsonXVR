
#include "AppImageGetter.h"
#include <Utils/AppHelper.h>
#include <Widgets/Layout/SScrollBox.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AppImageGetter)
#endif

UAppImageGetter::UAppImageGetter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), GeneratedImages(0u), DataSize(0u)
{
}

void UAppImageGetter::RequestSent()
{
	OnStatusChanged.ExecuteIfBound("Request Sent. Waiting for the response...");
}

void UAppImageGetter::RequestFailed()
{
	OnStatusChanged.ExecuteIfBound("Request Failed. Check the Logs.");
	Destroy();
}

void UAppImageGetter::ProcessCompleted(const FAppImageResponse& Response)
{
	if (!Response.bSuccess)
	{
		const FStringFormatOrderedArguments Arguments_ErrorDetails{
			FString("Request Failed."), FString("Please check the logs. (Enable internal logs in Project Settings -> Plugins -> App)."),
			FString("Error Details: "), FString("\tError Code: ") + Response.Error.Code.ToString(),
			FString("\tError Type: ") + Response.Error.Type.ToString(), FString("\tError Message: ") + Response.Error.Message
		};

		OnStatusChanged.ExecuteIfBound(FString::Format(TEXT("{0}\n{1}\n\n{2}\n{3}\n{4}\n{5}"), Arguments_ErrorDetails));
		Destroy();
		return;
	}

	DataSize = Response.Data.Num();
	OnStatusChanged.ExecuteIfBound("Request Completed.");

	OnImageGenerated_Internal.BindUFunction(this, TEXT("ImageGenerated"));

	for (const FAppImageData& Data : Response.Data)
	{
		ProcessImage(Data);
	}
}

void UAppImageGetter::ProcessImage(const FAppImageData& Data)
{
	UAppImageHelper::GenerateImage(Data, OnImageGenerated_Internal);
}

void UAppImageGetter::ImageGenerated(UTexture2D* const Texture)
{
	OnImageGenerated.ExecuteIfBound(Texture);

	++GeneratedImages;
	if (GeneratedImages >= DataSize)
	{
		if (OutScrollBox.IsValid())
		{
			OutScrollBox->ScrollToEnd();
		}

		Destroy();
	}
}

void UAppImageGetter::Destroy()
{
	ClearFlags(RF_Standalone);

#if ENGINE_MAJOR_VERSION >= 5
	MarkAsGarbage();
#else
    MarkPendingKill();
#endif
}
