



#include "Structures/WatsonxVRCommonTypes.h"
#include <Management/WatsonxVRSettings.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(WatsonxVRCommonTypes)
#endif

FWatsonxVRCommonOptions::FWatsonxVRCommonOptions()
{
	SetDefaults();
}

void FWatsonxVRCommonOptions::SetDefaults()
{
	if (const UWatsonxVRSettings* const Settings = GetDefault<UWatsonxVRSettings>())
	{
		APIKey = Settings->CommonOptions.APIKey;
		User = Settings->CommonOptions.User;
		bIsAzureOpenAI = Settings->CommonOptions.bIsAzureOpenAI;
		Endpoint = Settings->CommonOptions.Endpoint;
		AzureOpenAIAPIVersion = Settings->CommonOptions.AzureOpenAIAPIVersion;
	}
}
