



#include "Structures/WatsonxVRChatTypes.h"
#include "Utils/WatsonxVRHelper.h"
#include <Management/WatsonxVRSettings.h>
#include <Dom/JsonObject.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(WatsonxVRChatTypes)
#endif

TSharedPtr<FJsonValue> FWatsonxVRFunction::GetFunction() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("name", Name.ToString());
	JsonObject->SetStringField("description", Description);

	const TSharedPtr<FJsonObject> ParametersObject = MakeShared<FJsonObject>();
	ParametersObject->SetStringField("type", "object");

	const TSharedPtr<FJsonObject> PropertiesObject = MakeShared<FJsonObject>();
	for (const FWatsonxVRFunctionProperty& PropIt : Properties)
	{
		TSharedPtr<FJsonObject> PropertyObject = MakeShared<FJsonObject>();
		PropertyObject->SetStringField("type", UWatsonxVRHelper::PropertyTypeToName(PropIt.Type).ToString().ToLower());
		PropertyObject->SetStringField("description", PropIt.Description);

		TArray<TSharedPtr<FJsonValue>> EnumArr;
		for (const FName& EnumIt : PropIt.Enum)
		{
			EnumArr.Emplace(MakeShared<FJsonValueString>(EnumIt.ToString()));
		}
		PropertyObject->SetArrayField("enum", EnumArr);

		PropertiesObject->SetObjectField(PropIt.Name.ToString(), PropertyObject);
	}

	ParametersObject->SetObjectField("properties", PropertiesObject);

	TArray<TSharedPtr<FJsonValue>> RequiredParams;
	for (const FName& ReqIt : RequiredProperties)
	{
		RequiredParams.Emplace(MakeShared<FJsonValueString>(ReqIt.ToString()));
	}

	ParametersObject->SetArrayField("required", RequiredParams);
	JsonObject->SetObjectField("parameters", ParametersObject);

	return MakeShared<FJsonValueObject>(JsonObject);
}

FWatsonxVRChatMessage::FWatsonxVRChatMessage(const FName& InRole, const FString& InContent)
{
	Role = UWatsonxVRHelper::NameToRole(InRole);
	Content = InContent;
}

TSharedPtr<FJsonValue> FWatsonxVRChatMessage::GetMessage() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("role", UWatsonxVRHelper::RoleToName(Role).ToString().ToLower());

	if (Role == EWatsonxVRChatRole::Function)
	{
		JsonObject->SetStringField("name", FunctionCall.Name.ToString());
		JsonObject->SetStringField("content", FunctionCall.Arguments);
	}
	else
	{
		JsonObject->SetStringField("content", Content);
	}

	return MakeShared<FJsonValueObject>(JsonObject);
}

FWatsonxVRChatOptions::FWatsonxVRChatOptions()
{
	SetDefaults();
}

void FWatsonxVRChatOptions::SetDefaults()
{
	if (const UWatsonxVRSettings* const Settings = GetDefault<UWatsonxVRSettings>())
	{
		Model = Settings->ChatOptions.Model;
		MaxTokens = Settings->ChatOptions.MaxTokens;
		Temperature = Settings->ChatOptions.Temperature;
		TopP = Settings->ChatOptions.TopP;
		Choices = Settings->ChatOptions.Choices;
		bStream = Settings->ChatOptions.bStream;
		Stop = Settings->ChatOptions.Stop;
		PresencePenalty = Settings->ChatOptions.PresencePenalty;
		FrequencyPenalty = Settings->ChatOptions.FrequencyPenalty;
		LogitBias = Settings->ChatOptions.LogitBias;
	}
}
