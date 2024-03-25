

#include "AppMessagingHandler.h"
#include <AppInternalFuncs.h>
#include <Widgets/Layout/SScrollBox.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AppMessagingHandler)
#endif

UAppMessagingHandler::UAppMessagingHandler(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAppMessagingHandler::RequestSent()
{
	OnMessageContentUpdated.ExecuteIfBound("Waiting for response...");
}

void UAppMessagingHandler::RequestFailed()
{
	OnMessageContentUpdated.ExecuteIfBound(
		"Request Failed.\nPlease check the logs. (Enable internal logs in Project Settings -> Plugins -> App).");
	Destroy();
}

void UAppMessagingHandler::ProcessUpdated(const FAppChatResponse& Response)
{
	ProcessResponse(Response);
}

void UAppMessagingHandler::ProcessCompleted(const FAppChatResponse& Response)
{
	ProcessResponse(Response);
	Destroy();
}

void UAppMessagingHandler::ProcessResponse(const FAppChatResponse& Response)
{
	bool bScrollToEnd = false;
	if (ScrollBoxReference.IsValid())
	{
		bScrollToEnd = FMath::Abs(ScrollBoxReference->GetScrollOffsetOfEnd() - ScrollBoxReference->GetScrollOffset()) <= 8.f;
	}

	if (!Response.bSuccess)
	{
		const FStringFormatOrderedArguments Arguments_ErrorDetails{
			FString("Request Failed."), FString("Please check the logs. (Enable internal logs in Project Settings -> Plugins -> App)."),
			FString("Error Details: "), FString("\tError Code: ") + Response.Error.Code.ToString(),
			FString("\tError Type: ") + Response.Error.Type.ToString(), FString("\tError Message: ") + Response.Error.Message
		};

		OnMessageContentUpdated.ExecuteIfBound(FString::Format(TEXT("{0}\n{1}\n\n{2}\n{3}\n{4}\n{5}"), Arguments_ErrorDetails));
	}
	else if (Response.bSuccess && !App::Internal::HasEmptyParam(Response.Choices))
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

void UAppMessagingHandler::Destroy()
{
	ClearFlags(RF_Standalone);

#if ENGINE_MAJOR_VERSION >= 5
	MarkAsGarbage();
#else
    MarkPendingKill();
#endif
}
