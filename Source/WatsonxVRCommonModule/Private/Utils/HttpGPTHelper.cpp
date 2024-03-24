



#include "Utils/WatsonxVRHelper.h"
#include "WatsonxVRInternalFuncs.h"

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(WatsonxVRHelper)
#endif

const FName UWatsonxVRHelper::ModelToName(const EWatsonxVRChatModel Model)
{
	switch (Model)
	{
	case EWatsonxVRChatModel::gpt4:
		return "gpt-4";

	case EWatsonxVRChatModel::gpt432k:
		return "gpt-4-32k";

	case EWatsonxVRChatModel::gpt35turbo:
		return "gpt-3.5-turbo";

	case EWatsonxVRChatModel::gpt35turbo16k:
		return "gpt-3.5-turbo-16k";

	case EWatsonxVRChatModel::textdavinci003:
		return "text-davinci-003";

	case EWatsonxVRChatModel::textdavinci002:
		return "text-davinci-002";

	case EWatsonxVRChatModel::codedavinci002:
		return "code-davinci-002";

	default: break;
	}

	return NAME_None;
}

const EWatsonxVRChatModel UWatsonxVRHelper::NameToModel(const FName Model)
{
	if (Model.IsEqual("gpt-4", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatModel::gpt4;
	}
	if (Model.IsEqual("gpt-4-32k", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatModel::gpt432k;
	}
	if (Model.IsEqual("gpt-3.5-turbo", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatModel::gpt35turbo;
	}
	if (Model.IsEqual("gpt-3.5-turbo-16k", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatModel::gpt35turbo16k;
	}
	if (Model.IsEqual("text-davinci-003", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatModel::textdavinci003;
	}
	if (Model.IsEqual("text-davinci-002", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatModel::textdavinci002;
	}
	if (Model.IsEqual("code-davinci-002", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatModel::codedavinci002;
	}

	return EWatsonxVRChatModel::gpt35turbo;
}

const FName UWatsonxVRHelper::RoleToName(const EWatsonxVRChatRole Role)
{
	switch (Role)
	{
	case EWatsonxVRChatRole::Assistant:
		return "assistant";

	case EWatsonxVRChatRole::User:
		return "user";

	case EWatsonxVRChatRole::System:
		return "system";

	case EWatsonxVRChatRole::Function:
		return "function";

	default:
		break;
	}

	return NAME_None;
}

const EWatsonxVRChatRole UWatsonxVRHelper::NameToRole(const FName Role)
{
	if (Role.IsEqual("user", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatRole::User;
	}
	if (Role.IsEqual("assistant", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatRole::Assistant;
	}
	if (Role.IsEqual("system", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatRole::System;
	}
	if (Role.IsEqual("function", ENameCase::IgnoreCase))
	{
		return EWatsonxVRChatRole::Function;
	}

	return EWatsonxVRChatRole::User;
}

const FName UWatsonxVRHelper::PropertyTypeToName(const EWatsonxVRPropertyType Type)
{
	switch (Type)
	{
	case EWatsonxVRPropertyType::Boolean:
		return "bool";

	case EWatsonxVRPropertyType::Number:
		return "number";

	case EWatsonxVRPropertyType::String:
		return "string";

	default:
		break;
	}

	return NAME_None;
}

const EWatsonxVRPropertyType UWatsonxVRHelper::NameToPropertyType(const FName Type)
{
	if (Type.IsEqual("bool", ENameCase::IgnoreCase))
	{
		return EWatsonxVRPropertyType::Boolean;
	}
	if (Type.IsEqual("number", ENameCase::IgnoreCase))
	{
		return EWatsonxVRPropertyType::Number;
	}
	if (Type.IsEqual("string", ENameCase::IgnoreCase))
	{
		return EWatsonxVRPropertyType::String;
	}

	return EWatsonxVRPropertyType::Boolean;
}

const TArray<FName> UWatsonxVRHelper::GetAvailableGPTModels()
{
	TArray<FName> Output;

	for (uint8 Iterator = static_cast<uint8>(EWatsonxVRChatModel::gpt4); Iterator <= static_cast<uint8>(EWatsonxVRChatModel::codedavinci002); ++Iterator)
	{
		if (const FName ModelName = ModelToName(static_cast<EWatsonxVRChatModel>(Iterator)); !WatsonxVR::Internal::HasEmptyParam(ModelName))
		{
			Output.Add(ModelName);
		}
	}

	return Output;
}

const FString UWatsonxVRHelper::GetEndpointForModel(const EWatsonxVRChatModel Model, const bool bIsAzureOpenAI, const FString& AzureOpenAIAPIVersion)
{
	switch (Model)
	{
	case EWatsonxVRChatModel::gpt4:
	case EWatsonxVRChatModel::gpt432k:
	case EWatsonxVRChatModel::gpt35turbo:
	case EWatsonxVRChatModel::gpt35turbo16k:
		{
			if (bIsAzureOpenAI)
			{
				return FString::Format(
					TEXT("/openai/deployments/{0}/chat/completions?api-version={1}"), {ModelToName(Model).ToString(), AzureOpenAIAPIVersion});
			}
			return "v1/chat/completions";
		}

	case EWatsonxVRChatModel::textdavinci003:
	case EWatsonxVRChatModel::textdavinci002:
	case EWatsonxVRChatModel::codedavinci002:
		{
			if (bIsAzureOpenAI)
			{
				return FString::Format(
					TEXT("/openai/deployments/{0}/completions?api-version={1}"), {ModelToName(Model).ToString(), AzureOpenAIAPIVersion});
			}
			return "v1/completions";
		}

	default: break;
	}

	return FString();
}

const bool UWatsonxVRHelper::ModelSupportsChat(const EWatsonxVRChatModel Model)
{
	switch (Model)
	{
	case EWatsonxVRChatModel::gpt4:
	case EWatsonxVRChatModel::gpt432k:
	case EWatsonxVRChatModel::gpt35turbo:
	case EWatsonxVRChatModel::gpt35turbo16k:
		return true;

	case EWatsonxVRChatModel::textdavinci003:
	case EWatsonxVRChatModel::textdavinci002:
	case EWatsonxVRChatModel::codedavinci002:
		return false;

	default: break;
	}

	return false;
}

const FName UWatsonxVRHelper::SizeToName(const EWatsonxVRImageSize Size)
{
	switch (Size)
	{
	case EWatsonxVRImageSize::x256:
		return "256x256";

	case EWatsonxVRImageSize::x512:
		return "512x512";

	case EWatsonxVRImageSize::x1024:
		return "1024x1024";

	default:
		break;
	}

	return NAME_None;
}

const EWatsonxVRImageSize UWatsonxVRHelper::NameToSize(const FName Size)
{
	if (Size.IsEqual("256x256", ENameCase::IgnoreCase))
	{
		return EWatsonxVRImageSize::x256;
	}
	if (Size.IsEqual("512x512", ENameCase::IgnoreCase))
	{
		return EWatsonxVRImageSize::x512;
	}
	if (Size.IsEqual("1024x1024", ENameCase::IgnoreCase))
	{
		return EWatsonxVRImageSize::x1024;
	}

	return EWatsonxVRImageSize::x256;
}

const FName UWatsonxVRHelper::FormatToName(const EWatsonxVRResponseFormat Format)
{
	switch (Format)
	{
	case EWatsonxVRResponseFormat::url:
		return "url";

	case EWatsonxVRResponseFormat::b64_json:
		return "b64_json";

	default:
		break;
	}

	return NAME_None;
}

const EWatsonxVRResponseFormat UWatsonxVRHelper::NameToFormat(const FName Format)
{
	if (Format.IsEqual("url", ENameCase::IgnoreCase))
	{
		return EWatsonxVRResponseFormat::url;
	}
	if (Format.IsEqual("b64_json", ENameCase::IgnoreCase))
	{
		return EWatsonxVRResponseFormat::b64_json;
	}

	return EWatsonxVRResponseFormat::url;
}
