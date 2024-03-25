

Chat

// These structures are defined in the common module due to being used in both modules, to avoid cyclic dependencies.

#pragma once

#include <CoreMinimal.h>
#include <Dom/JsonValue.h>
#include "Structures/AppCommonTypes.h"
#include "AppChatTypes.generated.h"

UENUM(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Property Type"))
enum class EAppPropertyType : uint8
{
	Number,
	Boolean,
	String
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Function Parameter"))
struct AppCOMMONMODULE_API FAppFunctionProperty
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Name"))
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Type"))
	EAppPropertyType Type = EAppPropertyType::String;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Description"))
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Enum"))
	TArray<FName> Enum;
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Function"))
struct AppCOMMONMODULE_API FAppFunction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Name"))
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Description"))
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Properties"))
	TArray<FAppFunctionProperty> Properties;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Required Properties"))
	TArray<FName> RequiredProperties;

	TSharedPtr<FJsonValue> GetFunction() const;
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Function Call"))
struct AppCOMMONMODULE_API FAppFunctionCall
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Name"))
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Arguments"))
	FString Arguments;
};

UENUM(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Chat Role"))
enum class EAppChatRole : uint8
{
	User,
	Assistant,
	System,
	Function
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Chat Message"))
struct AppCOMMONMODULE_API FAppChatMessage
{
	GENERATED_BODY()

	FAppChatMessage() = default;

	FAppChatMessage(const EAppChatRole& InRole, const FString& InContent) : Role(InRole), Content(InContent)
	{
	}

	FAppChatMessage(const FName& InRole, const FString& InContent);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	EAppChatRole Role = EAppChatRole::User;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	FString Content;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (EditCondition = "Role == EAppChatRole::Function"))
	FAppFunctionCall FunctionCall;

	TSharedPtr<FJsonValue> GetMessage() const;
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Chat Choice"))
struct AppCOMMONMODULE_API FAppChatChoice
{
	GENERATED_BODY()

	FAppChatChoice() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	FAppChatMessage Message;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	FName FinishReason = NAME_None;
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Chat Usage"))
struct AppCOMMONMODULE_API FAppChatUsage
{
	GENERATED_BODY()

	FAppChatUsage() = default;

	FAppChatUsage(const int32& PromptTokens, const int32& CompletionTokens, const int32& TotalTokens) : PromptTokens(PromptTokens),
		CompletionTokens(CompletionTokens), TotalTokens(TotalTokens)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	int32 PromptTokens = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	int32 CompletionTokens = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	int32 TotalTokens = 0;
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Chat Response"))
struct AppCOMMONMODULE_API FAppChatResponse
{
	GENERATED_BODY()

	FAppChatResponse() = default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	FName ID = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	FName Object = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	int32 Created = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	TArray<FAppChatChoice> Choices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	FAppChatUsage Usage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	bool bSuccess = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat")
	FAppCommonError Error;
};

UENUM(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Chat Model"))
enum class EAppChatModel : uint8
{
	gpt4 UMETA(DisplayName = "gpt-4"),
	gpt432k UMETA(DisplayName = "gpt-4-32k"),
	gpt35turbo UMETA(DisplayName = "gpt-3.5-turbo"),
	gpt35turbo16k UMETA(DisplayName = "gpt-3.5-turbo-16k"),
	textdavinci003 UMETA(DisplayName = "text-davinci-003"),
	textdavinci002 UMETA(DisplayName = "text-davinci-002"),
	codedavinci002 UMETA(DisplayName = "code-davinci-002"),
};

USTRUCT(BlueprintType, Category = "App | Chat", Meta = (DisplayName = "App Chat Options"))
struct AppCOMMONMODULE_API FAppChatOptions
{
	GENERATED_BODY()

	FAppChatOptions();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Model"))
	EAppChatModel Model;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat",
		Meta = (DisplayName = "Temperature", ClampMin = "0.0", UIMin = "0.0", ClampMax = "2.0", UIMax = "2.0"))
	float Temperature;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat",
		Meta = (DisplayName = "TopP", ClampMin = "0.01", UIMin = "0.01", ClampMax = "1.0", UIMax = "1.0"))
	float TopP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Choices", ClampMin = "1", UIMin = "1"))
	int32 Choices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Stream"))
	bool bStream;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Stop"))
	TArray<FName> Stop;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat",
		Meta = (DisplayName = "Presence Penalty", ClampMin = "-2.0", UIMin = "-2.0", ClampMax = "2.0", UIMax = "2.0"))
	float PresencePenalty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat",
		Meta = (DisplayName = "Frequency Penalty", ClampMin = "-2.0", UIMin = "-2.0", ClampMax = "2.0", UIMax = "2.0"))
	float FrequencyPenalty;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat",
		Meta = (DisplayName = "Max Tokens", ClampMin = "1", UIMin = "1", ClampMax = "32768", UIMax = "32768"))
	int32 MaxTokens;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "App | Chat", Meta = (DisplayName = "Logit Bias"))
	TMap<int32, float> LogitBias;

private:
	void SetDefaults();
};
