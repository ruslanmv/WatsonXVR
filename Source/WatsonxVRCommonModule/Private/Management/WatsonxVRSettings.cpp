

#include "Management/WatsonxVRSettings.h"
#include "LogWatsonxVR.h"
#include <Runtime/Launch/Resources/Version.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(WatsonxVRSettings)
#endif

UWatsonxVRSettings::UWatsonxVRSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bUseCustomSystemContext(false),
                                                                                  CustomSystemContext(FString()),
                                                                                  GeneratedImagesDir("WatsonxVR_Generated"), bEnableInternalLogs(false)
{
	CategoryName = TEXT("Plugins");

	SetToDefaults();
}

const UWatsonxVRSettings* UWatsonxVRSettings::Get()
{
	const UWatsonxVRSettings* const Instance = GetDefault<UWatsonxVRSettings>();
	return Instance;
}

FWatsonxVRCommonOptions UWatsonxVRSettings::GetCommonOptions()
{
	return GetDefault<UWatsonxVRSettings>()->CommonOptions;
}

void UWatsonxVRSettings::SetCommonOptions(const FWatsonxVRCommonOptions& Value)
{
	UWatsonxVRSettings* const Settings = GetMutableDefault<UWatsonxVRSettings>();
	Settings->CommonOptions = Value;

	Settings->SaveAndReload(GET_MEMBER_NAME_CHECKED(UWatsonxVRSettings, CommonOptions));
}

FWatsonxVRChatOptions UWatsonxVRSettings::GetChatOptions()
{
	return GetDefault<UWatsonxVRSettings>()->ChatOptions;
}

void UWatsonxVRSettings::SetChatOptions(const FWatsonxVRChatOptions& Value)
{
	UWatsonxVRSettings* const Settings = GetMutableDefault<UWatsonxVRSettings>();
	Settings->ChatOptions = Value;

	Settings->SaveAndReload(GET_MEMBER_NAME_CHECKED(UWatsonxVRSettings, ChatOptions));
}

FWatsonxVRImageOptions UWatsonxVRSettings::GetImageOptions()
{
	return GetDefault<UWatsonxVRSettings>()->ImageOptions;
}

void UWatsonxVRSettings::SetImageOptions(const FWatsonxVRImageOptions& Value)
{
	UWatsonxVRSettings* const Settings = GetMutableDefault<UWatsonxVRSettings>();
	Settings->ImageOptions = Value;

	Settings->SaveAndReload(GET_MEMBER_NAME_CHECKED(UWatsonxVRSettings, ImageOptions));
}

#if WITH_EDITOR
void UWatsonxVRSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UWatsonxVRSettings, bEnableInternalLogs))
	{
		ToggleInternalLogs();
	}
}
#endif

void UWatsonxVRSettings::PostInitProperties()
{
	Super::PostInitProperties();
	ToggleInternalLogs();
}

void UWatsonxVRSettings::SetToDefaults()
{
	CommonOptions.APIKey = NAME_None;
	CommonOptions.User = NAME_None;
	CommonOptions.bIsAzureOpenAI = false;
	CommonOptions.Endpoint = TEXT("https://api.openai.com/");
	CommonOptions.AzureOpenAIAPIVersion = TEXT("2023-05-15");

	ChatOptions.Model = EWatsonxVRChatModel::gpt35turbo;
	ChatOptions.MaxTokens = 2048;
	ChatOptions.Temperature = 1.f;
	ChatOptions.TopP = 1.f;
	ChatOptions.Choices = 1;
	ChatOptions.bStream = true;
	ChatOptions.Stop = TArray<FName>();
	ChatOptions.PresencePenalty = 0.f;
	ChatOptions.FrequencyPenalty = 0.f;
	ChatOptions.LogitBias = TMap<int32, float>();

	ImageOptions.ImagesNum = 1;
	ImageOptions.Size = EWatsonxVRImageSize::x256;
	ImageOptions.Format = EWatsonxVRResponseFormat::url;
}

void UWatsonxVRSettings::SaveAndReload(const FName& PropertyName)
{
	SaveConfig();

	uint32 PropagationFlags = 0u;

#if ENGINE_MAJOR_VERSION >= 5
	PropagationFlags = UE::ELoadConfigPropagationFlags::LCPF_PropagateToChildDefaultObjects;
#else
    PropagationFlags = UE4::ELoadConfigPropagationFlags::LCPF_PropagateToChildDefaultObjects;
#endif

	ReloadConfig(GetClass(), *GetDefaultConfigFilename(), PropagationFlags, GetClass()->FindPropertyByName(PropertyName));
}

void UWatsonxVRSettings::ToggleInternalLogs()
{
#if !UE_BUILD_SHIPPING
	LogWatsonxVR_Internal.SetVerbosity(bEnableInternalLogs ? ELogVerbosity::Display : ELogVerbosity::NoLogging);
#endif
}
