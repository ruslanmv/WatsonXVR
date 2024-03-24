

#include "WatsonxVRMessagingHandler.h"
#include <WatsonxVRInternalFuncs.h>
#include <Widgets/Layout/SScrollBox.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(WatsonxVRMessagingHandler)
#endif

UWatsonxVRMessagingHandler::UWatsonxVRMessagingHandler(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWatsonxVRMessagingHandler::RequestSent()
{
	OnMessageContentUpdated.ExecuteIfBound("Waiting for response...");
}

void UWatsonxVRMessagingHandler::RequestFailed()
{
	OnMessageContentUpdated.ExecuteIfBound(
		"Request Failed.\nPlease check the logs. (Enable internal logs in Project Settings -> Plugins -> WatsonxVR).");
	Destroy();
}

void UWatsonxVRMessagingHandler::ProcessUpdated(const FWatsonxVRChatResponse& Response)
{
	ProcessResponse(Response);
}

void UWatsonxVRMessagingHandler::ProcessCompleted(const FWatsonxVRChatResponse& Response)
{
	ProcessResponse(Response);
	Destroy();
}

void UWatsonxVRMessagingHandler::ProcessResponse(const FWatsonxVRChatResponse& Response)
{
	bool bScrollToEnd = false;
	if (ScrollBoxReference.IsValid())
	{
		bScrollToEnd = FMath::Abs(ScrollBoxReference->GetScrollOffsetOfEnd() - ScrollBoxReference->GetScrollOffset()) <= 8.f;
	}

	if (!Response.bSuccess)
	{
		const FStringFormatOrderedArguments Arguments_ErrorDetails{
			FString("Request Failed."), FString("Please check the logs. (Enable internal logs in Project Settings -> Plugins -> WatsonxVR)."),
			FString("Error Details: "), FString("\tError Code: ") + Response.Error.Code.ToString(),
			FString("\tError Type: ") + Response.Error.Type.ToString(), FString("\tError Message: ") + Response.Error.Message
		};

		OnMessageContentUpdated.ExecuteIfBound(FString::Format(TEXT("{0}\n{1}\n\n{2}\n{3}\n{4}\n{5}"), Arguments_ErrorDetails));
	}
	else if (Response.bSuccess && !WatsonxVR::Internal::HasEmptyParam(Response.Choices))
	{
		OnMessageContentUpdated.ExecuteIfBound(Response.Choices[0].Message.Content);
	}
	else
	{
		return;
	}

	if (ScrollBoxReference.IsValid() && bScrollToEnd)
	{
		ScrollBoxReference->ScrollToEnd();
	}
}

void UWatsonxVRMessagingHandler::Destroy()
{
	ClearFlags(RF_Standalone);

#if ENGINE_MAJOR_VERSION >= 5
	MarkAsGarbage();
#else
    MarkPendingKill();
#endif
}
