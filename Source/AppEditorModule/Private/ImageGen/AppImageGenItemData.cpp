

#include "SAppImageGenItemData.h"
#include <Management/AppSettings.h>
#include <Brushes/SlateImageBrush.h>
#include <AssetRegistry/AssetRegistryModule.h>
#include <UObject/SavePackage.h>

void SAppImageGenItemData::Construct(const FArguments& InArgs)
{
	Texture = InArgs._Texture;

	ChildSlot
	[
		ConstructContent()
	];
}

TSharedRef<SWidget> SAppImageGenItemData::ConstructContent()
{
	constexpr float SlotPadding = 4.0f;

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot().Padding(SlotPadding).FillHeight(1.f)
		[
			SNew(SImage).Image(Texture.IsValid() ? new FSlateImageBrush(Texture.Get(), FVector2D(256.f, 256.f)) : nullptr)
		]
		+ SVerticalBox::Slot().Padding(SlotPadding).AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Save")))
			.HAlign(HAlign_Center)
			.OnClicked(this, &SAppImageGenItemData::HandleSaveButton)
			.IsEnabled(this, &SAppImageGenItemData::IsSaveEnabled)
		];
}

FReply SAppImageGenItemData::HandleSaveButton()
{
	const FString AssetName = FString::FromInt(Texture->GetUniqueID());
	FString TargetFilename = FPaths::Combine(TEXT("/Game/"), UAppSettings::Get()->GeneratedImagesDir, AssetName);
	FPaths::NormalizeFilename(TargetFilename);

	UPackage* const Package = CreatePackage(*TargetFilename);
	UTexture2D* const SavedTexture = NewObject<UTexture2D>(Package, *AssetName, RF_Public | RF_Standalone);

#if ENGINE_MAJOR_VERSION >= 5
	SavedTexture->SetPlatformData(Texture->GetPlatformData());
#else
    SavedTexture->PlatformData = Texture->PlatformData;
#endif

	SavedTexture->UpdateResource();

#if ENGINE_MAJOR_VERSION >= 5
	SavedTexture->Source.Init(Texture->GetSizeX(), Texture->GetSizeY(), 1, Texture->GetPlatformData()->Mips.Num(), TSF_BGRA8);
#else
    SavedTexture->Source.Init(Texture->GetSizeX(), Texture->GetSizeY(), 1, Texture->PlatformData->Mips.Num(), ETextureSourceFormat::TSF_BGRA8);
#endif

	SavedTexture->PostEditChange();

	SavedTexture->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(SavedTexture);

	const FString TempPackageFilename = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());

#if ENGINE_MAJOR_VERSION >= 5
	FSavePackageArgs SaveArgs;
	SaveArgs.SaveFlags = RF_Public | RF_Standalone;
	UPackage::SavePackage(Package, SavedTexture, *TempPackageFilename, SaveArgs);
#else
    UPackage::SavePackage(Package, SavedTexture, RF_Public | RF_Standalone, *TempPackageFilename);
#endif

	TArray<FAssetData> SyncAssets;
	SyncAssets.Add(FAssetData(SavedTexture));
	GEditor->SyncBrowserToObjects(SyncAssets);

	return FReply::Handled();
}

bool SAppImageGenItemData::IsSaveEnabled() const
{
	return Texture.IsValid();
}
