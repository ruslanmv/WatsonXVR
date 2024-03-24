



#include "Tasks/WatsonxVRImageRequest.h"
#include <Utils/WatsonxVRHelper.h>
#include <Management/WatsonxVRSettings.h>
#include <WatsonxVRInternalFuncs.h>
#include <LogWatsonxVR.h>

#include <HttpModule.h>
#include <Interfaces/IHttpRequest.h>
#include <Interfaces/IHttpResponse.h>
#include <Dom/JsonObject.h>
#include <Serialization/JsonWriter.h>
#include <Serialization/JsonReader.h>
#include <Serialization/JsonSerializer.h>
#include <Misc/ScopeTryLock.h>
#include <Async/Async.h>
#include <Engine/Texture2D.h>
#include <Misc/Base64.h>
#include <ImageUtils.h>

#if WITH_EDITOR
#include <Editor.h>
#endif

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(WatsonxVRImageRequest)
#endif

#if WITH_EDITOR
UWatsonxVRImageRequest* UWatsonxVRImageRequest::EditorTask(const FString& Prompt, const FWatsonxVRImageOptions Options)
{
	UWatsonxVRImageRequest* const NewAsyncTask = RequestImages_CustomOptions(GEditor->GetEditorWorldContext().World(), Prompt, FWatsonxVRCommonOptions(),
	                                                                       Options);
	NewAsyncTask->bIsEditorTask = true;

	return NewAsyncTask;
}
#endif

UWatsonxVRImageRequest* UWatsonxVRImageRequest::RequestImages_DefaultOptions(UObject* const WorldContextObject, const FString& Prompt)
{
	return RequestImages_CustomOptions(WorldContextObject, Prompt, FWatsonxVRCommonOptions(), FWatsonxVRImageOptions());
}

UWatsonxVRImageRequest* UWatsonxVRImageRequest::RequestImages_CustomOptions(UObject* const WorldContextObject, const FString& Prompt,
                                                                        const FWatsonxVRCommonOptions CommonOptions,
                                                                        const FWatsonxVRImageOptions ImageOptions)
{
	UWatsonxVRImageRequest* const NewAsyncTask = NewObject<UWatsonxVRImageRequest>();
	NewAsyncTask->Prompt = Prompt;
	NewAsyncTask->CommonOptions = CommonOptions;
	NewAsyncTask->ImageOptions = ImageOptions;

	NewAsyncTask->RegisterWithGameInstance(WorldContextObject);

	return NewAsyncTask;
}

const FWatsonxVRImageOptions UWatsonxVRImageRequest::GetImageOptions() const
{
	return ImageOptions;
}

const FString UWatsonxVRImageRequest::GetPrompt() const
{
	return Prompt;
}

bool UWatsonxVRImageRequest::CanActivateTask() const
{
	if (!Super::CanActivateTask())
	{
		return false;
	}

	if (WatsonxVR::Internal::HasEmptyParam(Prompt))
	{
		UE_LOG(LogWatsonxVR, Error, TEXT("%s (%d): Can't activate task: Invalid Prompt."), *FString(__func__), GetUniqueID());
		return false;
	}

	return true;
}

bool UWatsonxVRImageRequest::CanBindProgress() const
{
	return false;
}

FString UWatsonxVRImageRequest::GetEndpointURL() const
{
	if (CommonOptions.bIsAzureOpenAI)
	{
		return FString::Format(
			TEXT("{0}/openai/images/generations:submit?api-version={1}"), {GetCommonOptions().Endpoint, GetCommonOptions().AzureOpenAIAPIVersion});
	}
	return FString::Format(TEXT("{0}/v1/images/generations"), {GetCommonOptions().Endpoint});
}

FString UWatsonxVRImageRequest::SetRequestContent()
{
	FScopeLock Lock(&Mutex);

	if (!HttpRequest.IsValid())
	{
		return FString();
	}

	UE_LOG(LogWatsonxVR_Internal, Display, TEXT("%s (%d): Mounting content"), *FString(__func__), GetUniqueID());

	const TSharedPtr<FJsonObject> JsonRequest = MakeShared<FJsonObject>();
	JsonRequest->SetStringField("prompt", Prompt);
	JsonRequest->SetNumberField("n", GetImageOptions().ImagesNum);
	JsonRequest->SetStringField("size", UWatsonxVRHelper::SizeToName(GetImageOptions().Size).ToString());
	JsonRequest->SetStringField("response_format", UWatsonxVRHelper::FormatToName(GetImageOptions().Format).ToString().ToLower());

	if (!WatsonxVR::Internal::HasEmptyParam(GetCommonOptions().User))
	{
		JsonRequest->SetStringField("user", GetCommonOptions().User.ToString());
	}

	FString RequestContentString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContentString);
	FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), Writer);

	HttpRequest->SetContentAsString(RequestContentString);

	return RequestContentString;
}

void UWatsonxVRImageRequest::OnProgressCompleted(const FString& Content, const bool bWasSuccessful)
{
	FScopeLock Lock(&Mutex);

	if (!bWasSuccessful || WatsonxVR::Internal::HasEmptyParam(Content))
	{
		UE_LOG(LogWatsonxVR, Error, TEXT("%s (%d): Request failed"), *FString(__func__), GetUniqueID());
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			RequestFailed.Broadcast();
		});

		return;
	}

	UE_LOG(LogWatsonxVR_Internal, Display, TEXT("%s (%d): Process Completed"), *FString(__func__), GetUniqueID());
	UE_LOG(LogWatsonxVR_Internal, Display, TEXT("%s (%d): Content: %s"), *FString(__func__), GetUniqueID(), *Content);

	DeserializeResponse(Content);

	if (Response.bSuccess)
	{
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			FScopeLock Lock(&Mutex);
			ProcessCompleted.Broadcast(Response);
		});
	}
	else
	{
		UE_LOG(LogWatsonxVR, Error, TEXT("%s (%d): Request failed"), *FString(__func__), GetUniqueID());
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			FScopeLock Lock(&Mutex);
			ErrorReceived.Broadcast(Response);
		});
	}
}

void UWatsonxVRImageRequest::DeserializeResponse(const FString& Content)
{
	FScopeLock Lock(&Mutex);

	if (WatsonxVR::Internal::HasEmptyParam(Content))
	{
		return;
	}

	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
	TSharedPtr<FJsonObject> JsonResponse = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(Reader, JsonResponse);

	if (CheckError(JsonResponse, Response.Error))
	{
		Response.bSuccess = false;
		return;
	}

	Response.bSuccess = true;
	Response.Created = JsonResponse->GetNumberField("created");

	const TArray<TSharedPtr<FJsonValue>> DataArray = JsonResponse->GetArrayField("data");
	for (auto Iterator = DataArray.CreateConstIterator(); Iterator; ++Iterator)
	{
		Response.Data.Add(FWatsonxVRImageData(
			(*Iterator)->AsObject()->GetStringField(UWatsonxVRHelper::FormatToName(GetImageOptions().Format).ToString()), GetImageOptions().Format));
	}
}

UWatsonxVRImageRequest* UWatsonxVRImageHelper::CastToWatsonxVRImageRequest(UObject* const Object)
{
	return Cast<UWatsonxVRImageRequest>(Object);
}

void UWatsonxVRImageHelper::GenerateImage(const FWatsonxVRImageData& ImageData, const FWatsonxVRImageGenerate& Callback)
{
	switch (ImageData.Format)
	{
	case EWatsonxVRResponseFormat::url:
		GenerateImageFromURL(ImageData, Callback);
		break;

	case EWatsonxVRResponseFormat::b64_json:
		GenerateImageFromB64(ImageData, Callback);
		break;

	default:
		break;
	}
}

void UWatsonxVRImageHelper::GenerateImageFromURL(const FWatsonxVRImageData& ImageData, const FWatsonxVRImageGenerate& Callback)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ImageData.Content);
	HttpRequest->SetVerb("GET");
	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, const FHttpResponsePtr& Response, bool bSuccess)
	{
		if (bSuccess && Response.IsValid())
		{
			Callback.ExecuteIfBound(FImageUtils::ImportBufferAsTexture2D(Response->GetContent()));
		}
		else
		{
			Callback.ExecuteIfBound(nullptr);
		}
	});
	HttpRequest->ProcessRequest();
}

void UWatsonxVRImageHelper::GenerateImageFromB64(const FWatsonxVRImageData& ImageData, const FWatsonxVRImageGenerate& Callback)
{
	TArray<uint8> DecodedBytes;
	FBase64::Decode(ImageData.Content, DecodedBytes);
	Callback.ExecuteIfBound(FImageUtils::ImportBufferAsTexture2D(DecodedBytes));
}
