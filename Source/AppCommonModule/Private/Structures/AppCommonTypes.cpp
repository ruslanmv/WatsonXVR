



#include "Structures/AppCommonTypes.h"
#include <Management/AppSettings.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AppCommonTypes)
#endif

FAppCommonOptions::FAppCommonOptions()
{
	SetDefaults();
}

void FAppCommonOptions::SetDefaults()
{
	if (const UAppSettings* const Settings = GetDefault<UAppSettings>())
	{
		APIKey = Settings->CommonOptions.APIKey;
		User = Settings->CommonOptions.User;
		bIsAzureOpenAI = Settings->CommonOptions.bIsAzureOpenAI;
		Endpoint = Settings->CommonOptions.Endpoint;
		AzureOpenAIAPIVersion = Settings->CommonOptions.AzureOpenAIAPIVersion;
	}
}
