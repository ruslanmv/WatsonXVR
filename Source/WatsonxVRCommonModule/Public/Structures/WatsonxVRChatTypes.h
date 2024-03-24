

Chat

// These structures are defined in the common module due to being used in both modules, to avoid cyclic dependencies.

#pragma once

#include <CoreMinimal.h>
#include <Dom/JsonValue.h>
#include "Structures/WatsonxVRCommonTypes.h"
#include "WatsonxVRChatTypes.generated.h"

UENUM(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Property Type"))
enum class EWatsonxVRPropertyType : uint8
{
	Number,
	Boolean,
	String
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Function Parameter"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRFunctionProperty
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Name"))
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Type"))
	EWatsonxVRPropertyType Type = EWatsonxVRPropertyType::String;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Description"))
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Enum"))
	TArray<FName> Enum;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Function"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRFunction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Name"))
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Description"))
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Properties"))
	TArray<FWatsonxVRFunctionProperty> Properties;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Required Properties"))
	TArray<FName> RequiredProperties;

	TSharedPtr<FJsonValue> GetFunction() const;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Function Call"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRFunctionCall
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Name"))
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Arguments"))
	FString Arguments;
};

UENUM(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Role"))
enum class EWatsonxVRChatRole : uint8
{
	User,
	Assistant,
	System,
	Function
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Message"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRChatMessage
{
	GENERATED_BODY()

	FWatsonxVRChatMessage() = default;

	FWatsonxVRChatMessage(const EWatsonxVRChatRole& InRole, const FString& InContent) : Role(InRole), Content(InContent)
	{
	}

	FWatsonxVRChatMessage(const FName& InRole, const FString& InContent);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	EWatsonxVRChatRole Role = EWatsonxVRChatRole::User;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	FString Content;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (EditCondition = "Role == EWatsonxVRChatRole::Function"))
	FWatsonxVRFunctionCall FunctionCall;

	TSharedPtr<FJsonValue> GetMessage() const;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Choice"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRChatChoice
{
	GENERATED_BODY()

	FWatsonxVRChatChoice() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	FWatsonxVRChatMessage Message;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	FName FinishReason = NAME_None;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Usage"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRChatUsage
{
	GENERATED_BODY()

	FWatsonxVRChatUsage() = default;

	FWatsonxVRChatUsage(const int32& PromptTokens, const int32& CompletionTokens, const int32& TotalTokens) : PromptTokens(PromptTokens),
		CompletionTokens(CompletionTokens), TotalTokens(TotalTokens)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	int32 PromptTokens = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	int32 CompletionTokens = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	int32 TotalTokens = 0;
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Response"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRChatResponse
{
	GENERATED_BODY()

	FWatsonxVRChatResponse() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	FName ID = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	FName Object = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	int32 Created = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	TArray<FWatsonxVRChatChoice> Choices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	FWatsonxVRChatUsage Usage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	bool bSuccess = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat")
	FWatsonxVRCommonError Error;
};

UENUM(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Model"))
enum class EWatsonxVRChatModel : uint8
{
	gpt4 UMETA(DisplayName = "gpt-4"),
	gpt432k UMETA(DisplayName = "gpt-4-32k"),
	gpt35turbo UMETA(DisplayName = "gpt-3.5-turbo"),
	gpt35turbo16k UMETA(DisplayName = "gpt-3.5-turbo-16k"),
	textdavinci003 UMETA(DisplayName = "text-davinci-003"),
	textdavinci002 UMETA(DisplayName = "text-davinci-002"),
	codedavinci002 UMETA(DisplayName = "code-davinci-002"),
};

USTRUCT(BlueprintType, Category = "WatsonxVR | Chat", Meta = (DisplayName = "WatsonxVR Chat Options"))
struct WatsonxVRCOMMONMODULE_API FWatsonxVRChatOptions
{
	GENERATED_BODY()

	FWatsonxVRChatOptions();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Model"))
	EWatsonxVRChatModel Model;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat",
		Meta = (DisplayName = "Temperature", ClampMin = "0.0", UIMin = "0.0", ClampMax = "2.0", UIMax = "2.0"))
	float Temperature;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat",
		Meta = (DisplayName = "TopP", ClampMin = "0.01", UIMin = "0.01", ClampMax = "1.0", UIMax = "1.0"))
	float TopP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Choices", ClampMin = "1", UIMin = "1"))
	int32 Choices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Stream"))
	bool bStream;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Stop"))
	TArray<FName> Stop;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat",
		Meta = (DisplayName = "Presence Penalty", ClampMin = "-2.0", UIMin = "-2.0", ClampMax = "2.0", UIMax = "2.0"))
	float PresencePenalty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat",
		Meta = (DisplayName = "Frequency Penalty", ClampMin = "-2.0", UIMin = "-2.0", ClampMax = "2.0", UIMax = "2.0"))
	float FrequencyPenalty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat",
		Meta = (DisplayName = "Max Tokens", ClampMin = "1", UIMin = "1", ClampMax = "32768", UIMax = "32768"))
	int32 MaxTokens;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WatsonxVR | Chat", Meta = (DisplayName = "Logit Bias"))
	TMap<int32, float> LogitBias;

private:
	void SetDefaults();
};
