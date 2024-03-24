
#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

class SWatsonxVRImageGenItemData final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWatsonxVRImageGenItemData) : _Texture()
		{
		}

		SLATE_ARGUMENT(class UTexture2D*, Texture)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply HandleSaveButton();
	bool IsSaveEnabled() const;

private:
	TSharedRef<SWidget> ConstructContent();

	TWeakObjectPtr<class UTexture2D> Texture;
};

using SWatsonxVRImageGenItemDataPtr = TSharedPtr<SWatsonxVRImageGenItemData>;
