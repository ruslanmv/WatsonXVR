



#include "Tasks/AppImageRequest.h"
#include <Utils/AppHelper.h>
#include <Management/AppSettings.h>
#include <AppInternalFuncs.h>
#include <LogApp.h>

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
#include UE_INLINE_GENERATED_CPP_BY_NAME(AppImageRequest)
#endif

#if WITH_EDITOR
UAppImageRequest* UAppImageRequest::EditorTask(const FString& Prompt, const FAppImageOptions Options)
{
	UAppImageRequest* const NewAsyncTask = RequestImages_CustomOptions(GEditor->GetEditorWorldContext().World(), Prompt, FAppCommonOptions(),
	                                                                       Options);
	NewAsyncTask->bIsEditorTask = true;

	return NewAsyncTask;
}
#endif

UAppImageRequest* UAppImageRequest::RequestImages_DefaultOptions(UObject* const WorldContextObject, const FString& Prompt)
{
	return RequestImages_CustomOptions(WorldContextObject, Prompt, FAppCommonOptions(), FAppImageOptions());
}

UAppImageRequest* UAppImageRequest::RequestImages_CustomOptions(UObject* const WorldContextObject, const FString& Prompt,
                                                                        const FAppCommonOptions CommonOptions,
                                                                        const FAppImageOptions ImageOptions)
{
	UAppImageRequest* const NewAsyncTask = NewObject<UAppImageRequest>();
	NewAsyncTask->Prompt = Prompt;
	NewAsyncTask->CommonOptions = CommonOptions;
	NewAsyncTask->ImageOptions = ImageOptions;

	NewAsyncTask->RegisterWithGameInstance(WorldContextObject);

	return NewAsyncTask;
}

const FAppImageOptions UAppImageRequest::GetImageOptions() const
{
	return ImageOptions;
}

const FString UAppImageRequest::GetPrompt() const
{
	return Prompt;
}

bool UAppImageRequest::CanActivateTask() const
{
	if (!Super::CanActivateTask())
	{
		return false;
	}

	if (App::Internal::HasEmptyParam(Prompt))
	{
		UE_LOG(LogApp, Error, TEXT("%s (%d): Can't activate task: Invalid Prompt."), *FString(__func__), GetUniqueID());
		return false;
	}

	return true;
}

bool UAppImageRequest::CanBindProgress() const
{
	return false;
}

FString UAppImageRequest::GetEndpointURL() const
{
	if (CommonOptions.bIsAzureOpenAI)
	{
		return FString::Format(
			TEXT("{0}/openai/images/generations:submit?api-version={1}"), {GetCommonOptions().Endpoint, GetCommonOptions().AzureOpenAIAPIVersion});
	}
	return FString::Format(TEXT("{0}/v1/images/generations"), {GetCommonOptions().Endpoint});
}

FString UAppImageRequest::SetRequestContent()
{
	FScopeLock Lock(&Mutex);

	if (!HttpRequest.IsValid())
	{
		return FString();
	}

	UE_LOG(LogApp_Internal, Display, TEXT("%s (%d): Mounting content"), *FString(__func__), GetUniqueID());

	const TSharedPtr<FJsonObject> JsonRequest = MakeShared<FJsonObject>();
	JsonRequest->SetStringField("prompt", Prompt);
	JsonRequest->SetNumberField("n", GetImageOptions().ImagesNum);
	JsonRequest->SetStringField("size", UAppHelper::SizeToName(GetImageOptions().Size).ToString());
	JsonRequest->SetStringField("response_format", UAppHelper::FormatToName(GetImageOptions().Format).ToString().ToLower());

	if (!App::Internal::HasEmptyParam(GetCommonOptions().User))
	{
		JsonRequest->SetStringField("user", GetCommonOptions().User.ToString());
	}

	FString RequestContentString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContentString);
	FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), Writer);

	HttpRequest->SetContentAsString(RequestContentString);

	return RequestContentString;
}

void UAppImageRequest::OnProgressCompleted(const FString& Content, const bool bWasSuccessful)
{
	FScopeLock Lock(&Mutex);

	if (!bWasSuccessful || App::Internal::HasEmptyParam(Content))
	{
		UE_LOG(LogApp, Error, TEXT("%s (%d): Request failed"), *FString(__func__), GetUniqueID());
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			RequestFailed.Broadcast();
		});

		return;
	}

	UE_LOG(LogApp_Internal, Display, TEXT("%s (%d): Process Completed"), *FString(__func__), GetUniqueID());
	UE_LOG(LogApp_Internal, Display, TEXT("%s (%d): Content: %s"), *FString(__func__), GetUniqueID(), *Content);

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
		UE_LOG(LogApp, Error, TEXT("%s (%d): Request failed"), *FString(__func__), GetUniqueID());
		AsyncTask(ENamedThreads::GameThread, [this]
		{
			FScopeLock Lock(&Mutex);
			ErrorReceived.Broadcast(Response);
		});
	}
}

void UAppImageRequest::DeserializeResponse(const FString& Content)
{
	FScopeLock Lock(&Mutex);

	if (App::Internal::HasEmptyParam(Content))
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
		Response.Data.Add(FAppImageData(
			(*Iterator)->AsObject()->GetStringField(UAppHelper::FormatToName(GetImageOptions().Format).ToString()), GetImageOptions().Format));
	}
}

UAppImageRequest* UAppImageHelper::CastToAppImageRequest(UObject* const Object)
{
	return Cast<UAppImageRequest>(Object);
}

void UAppImageHelper::GenerateImage(const FAppImageData& ImageData, const FAppImageGenerate& Callback)
{
	switch (ImageData.Format)
	{
	case EAppResponseFormat::url:
		GenerateImageFromURL(ImageData, Callback);
		break;

	case EAppResponseFormat::b64_json:
		GenerateImageFromB64(ImageData, Callback);
		break;

	default:
		break;
	}
}

void UAppImageHelper::GenerateImageFromURL(const FAppImageData& ImageData, const FAppImageGenerate& Callback)
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

void UAppImageHelper::GenerateImageFromB64(const FAppImageData& ImageData, const FAppImageGenerate& Callback)
{
	TArray<uint8> DecodedBytes;
	FBase64::Decode(ImageData.Content, DecodedBytes);
	Callback.ExecuteIfBound(FImageUtils::ImportBufferAsTexture2D(DecodedBytes));
}
