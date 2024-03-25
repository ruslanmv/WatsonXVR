
#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

class SAppImageGenItemData final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAppImageGenItemData) : _Texture()
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

using SAppImageGenItemDataPtr = TSharedPtr<SAppImageGenItemData>;
