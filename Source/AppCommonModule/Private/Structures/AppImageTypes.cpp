



#include "Structures/AppImageTypes.h"
#include "Management/AppSettings.h"

FAppImageOptions::FAppImageOptions()
{
	SetDefaults();
}

void FAppImageOptions::SetDefaults()
{
	if (const UAppSettings* const Settings = GetDefault<UAppSettings>())
	{
		ImagesNum = Settings->ImageOptions.ImagesNum;
		Size = Settings->ImageOptions.Size;
		Format = Settings->ImageOptions.Format;
	}
}
