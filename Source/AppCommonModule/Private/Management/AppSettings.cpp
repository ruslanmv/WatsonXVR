

#include "Management/AppSettings.h"
#include "LogApp.h"
#include <Runtime/Launch/Resources/Version.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AppSettings)
#endif

UAppSettings::UAppSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bUseCustomSystemContext(false),
                                                                                  CustomSystemContext(FString()),
                                                                                  GeneratedImagesDir("App_Generated"), bEnableInternalLogs(false)
{
	CategoryName = TEXT("Plugins");

	SetToDefaults();
}

const UAppSettings* UAppSettings::Get()
{
	const UAppSettings* const Instance = GetDefault<UAppSettings>();
	return Instance;
}

FAppCommonOptions UAppSettings::GetCommonOptions()
{
	return GetDefault<UAppSettings>()->CommonOptions;
}

void UAppSettings::SetCommonOptions(const FAppCommonOptions& Value)
{
	UAppSettings* const Settings = GetMutableDefault<UAppSettings>();
	Settings->CommonOptions = Value;

	Settings->SaveAndReload(GET_MEMBER_NAME_CHECKED(UAppSettings, CommonOptions));
}

FAppChatOptions UAppSettings::GetChatOptions()
{
	return GetDefault<UAppSettings>()->ChatOptions;
}

void UAppSettings::SetChatOptions(const FAppChatOptions& Value)
{
	UAppSettings* const Settings = GetMutableDefault<UAppSettings>();
	Settings->ChatOptions = Value;

	Settings->SaveAndReload(GET_MEMBER_NAME_CHECKED(UAppSettings, ChatOptions));
}

FAppImageOptions UAppSettings::GetImageOptions()
{
	return GetDefault<UAppSettings>()->ImageOptions;
}

void UAppSettings::SetImageOptions(const FAppImageOptions& Value)
{
	UAppSettings* const Settings = GetMutableDefault<UAppSettings>();
	Settings->ImageOptions = Value;

	Settings->SaveAndReload(GET_MEMBER_NAME_CHECKED(UAppSettings, ImageOptions));
}

#if WITH_EDITOR
void UAppSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAppSettings, bEnableInternalLogs))
	{
		ToggleInternalLogs();
	}
}
#endif

void UAppSettings::PostInitProperties()
{
	Super::PostInitProperties();
	ToggleInternalLogs();
}

void UAppSettings::SetToDefaults()
{
	CommonOptions.APIKey = NAME_None;
	CommonOptions.User = NAME_None;
	CommonOptions.bIsAzureOpenAI = false;
	CommonOptions.Endpoint = TEXT("https://api.openai.com/");
	CommonOptions.AzureOpenAIAPIVersion = TEXT("2023-05-15");

	ChatOptions.Model = EAppChatModel::gpt35turbo;
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
	ImageOptions.Size = EAppImageSize::x256;
	ImageOptions.Format = EAppResponseFormat::url;
}

void UAppSettings::SaveAndReload(const FName& PropertyName)
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

void UAppSettings::ToggleInternalLogs()
{
#if !UE_BUILD_SHIPPING
	LogApp_Internal.SetVerbosity(bEnableInternalLogs ? ELogVerbosity::Display : ELogVerbosity::NoLogging);
#endif
}
