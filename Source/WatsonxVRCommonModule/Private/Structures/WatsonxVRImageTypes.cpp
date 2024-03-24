



#include "Structures/WatsonxVRImageTypes.h"
#include "Management/WatsonxVRSettings.h"

FWatsonxVRImageOptions::FWatsonxVRImageOptions()
{
	SetDefaults();
}

void FWatsonxVRImageOptions::SetDefaults()
{
	if (const UWatsonxVRSettings* const Settings = GetDefault<UWatsonxVRSettings>())
	{
		ImagesNum = Settings->ImageOptions.ImagesNum;
		Size = Settings->ImageOptions.Size;
		Format = Settings->ImageOptions.Format;
	}
}
