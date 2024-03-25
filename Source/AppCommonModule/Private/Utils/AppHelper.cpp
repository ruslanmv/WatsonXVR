



#include "Utils/AppHelper.h"
#include "AppInternalFuncs.h"

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AppHelper)
#endif

const FName UAppHelper::ModelToName(const EAppChatModel Model)
{
	switch (Model)
	{
	case EAppChatModel::gpt4:
		return "gpt-4";

	case EAppChatModel::gpt432k:
		return "gpt-4-32k";

	case EAppChatModel::gpt35turbo:
		return "gpt-3.5-turbo";

	case EAppChatModel::gpt35turbo16k:
		return "gpt-3.5-turbo-16k";

	case EAppChatModel::textdavinci003:
		return "text-davinci-003";

	case EAppChatModel::textdavinci002:
		return "text-davinci-002";

	case EAppChatModel::codedavinci002:
		return "code-davinci-002";

	default: break;
	}

	return NAME_None;
}

const EAppChatModel UAppHelper::NameToModel(const FName Model)
{
	if (Model.IsEqual("gpt-4", ENameCase::IgnoreCase))
	{
		return EAppChatModel::gpt4;
	}
	if (Model.IsEqual("gpt-4-32k", ENameCase::IgnoreCase))
	{
		return EAppChatModel::gpt432k;
	}
	if (Model.IsEqual("gpt-3.5-turbo", ENameCase::IgnoreCase))
	{
		return EAppChatModel::gpt35turbo;
	}
	if (Model.IsEqual("gpt-3.5-turbo-16k", ENameCase::IgnoreCase))
	{
		return EAppChatModel::gpt35turbo16k;
	}
	if (Model.IsEqual("text-davinci-003", ENameCase::IgnoreCase))
	{
		return EAppChatModel::textdavinci003;
	}
	if (Model.IsEqual("text-davinci-002", ENameCase::IgnoreCase))
	{
		return EAppChatModel::textdavinci002;
	}
	if (Model.IsEqual("code-davinci-002", ENameCase::IgnoreCase))
	{
		return EAppChatModel::codedavinci002;
	}

	return EAppChatModel::gpt35turbo;
}

const FName UAppHelper::RoleToName(const EAppChatRole Role)
{
	switch (Role)
	{
	case EAppChatRole::Assistant:
		return "assistant";

	case EAppChatRole::User:
		return "user";

	case EAppChatRole::System:
		return "system";

	case EAppChatRole::Function:
		return "function";

	default:
		break;
	}

	return NAME_None;
}

const EAppChatRole UAppHelper::NameToRole(const FName Role)
{
	if (Role.IsEqual("user", ENameCase::IgnoreCase))
	{
		return EAppChatRole::User;
	}
	if (Role.IsEqual("assistant", ENameCase::IgnoreCase))
	{
		return EAppChatRole::Assistant;
	}
	if (Role.IsEqual("system", ENameCase::IgnoreCase))
	{
		return EAppChatRole::System;
	}
	if (Role.IsEqual("function", ENameCase::IgnoreCase))
	{
		return EAppChatRole::Function;
	}

	return EAppChatRole::User;
}

const FName UAppHelper::PropertyTypeToName(const EAppPropertyType Type)
{
	switch (Type)
	{
	case EAppPropertyType::Boolean:
		return "bool";

	case EAppPropertyType::Number:
		return "number";

	case EAppPropertyType::String:
		return "string";

	default:
		break;
	}

	return NAME_None;
}

const EAppPropertyType UAppHelper::NameToPropertyType(const FName Type)
{
	if (Type.IsEqual("bool", ENameCase::IgnoreCase))
	{
		return EAppPropertyType::Boolean;
	}
	if (Type.IsEqual("number", ENameCase::IgnoreCase))
	{
		return EAppPropertyType::Number;
	}
	if (Type.IsEqual("string", ENameCase::IgnoreCase))
	{
		return EAppPropertyType::String;
	}

	return EAppPropertyType::Boolean;
}

const TArray<FName> UAppHelper::GetAvailableGPTModels()
{
	TArray<FName> Output;

	for (uint8 Iterator = static_cast<uint8>(EAppChatModel::gpt4); Iterator <= static_cast<uint8>(EAppChatModel::codedavinci002); ++Iterator)
	{
		if (const FName ModelName = ModelToName(static_cast<EAppChatModel>(Iterator)); !App::Internal::HasEmptyParam(ModelName))
		{
			Output.Add(ModelName);
		}
	}

	return Output;
}

const FString UAppHelper::GetEndpointForModel(const EAppChatModel Model, const bool bIsAzureOpenAI, const FString& AzureOpenAIAPIVersion)
{
	switch (Model)
	{
	case EAppChatModel::gpt4:
	case EAppChatModel::gpt432k:
	case EAppChatModel::gpt35turbo:
	case EAppChatModel::gpt35turbo16k:
		{
			if (bIsAzureOpenAI)
			{
				return FString::Format(
					TEXT("/openai/deployments/{0}/chat/completions?api-version={1}"), {ModelToName(Model).ToString(), AzureOpenAIAPIVersion});
			}
			return "v1/chat/completions";
		}

	case EAppChatModel::textdavinci003:
	case EAppChatModel::textdavinci002:
	case EAppChatModel::codedavinci002:
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

const bool UAppHelper::ModelSupportsChat(const EAppChatModel Model)
{
	switch (Model)
	{
	case EAppChatModel::gpt4:
	case EAppChatModel::gpt432k:
	case EAppChatModel::gpt35turbo:
	case EAppChatModel::gpt35turbo16k:
		return true;

	case EAppChatModel::textdavinci003:
	case EAppChatModel::textdavinci002:
	case EAppChatModel::codedavinci002:
		return false;

	default: break;
	}

	return false;
}

const FName UAppHelper::SizeToName(const EAppImageSize Size)
{
	switch (Size)
	{
	case EAppImageSize::x256:
		return "256x256";

	case EAppImageSize::x512:
		return "512x512";

	case EAppImageSize::x1024:
		return "1024x1024";

	default:
		break;
	}

	return NAME_None;
}

const EAppImageSize UAppHelper::NameToSize(const FName Size)
{
	if (Size.IsEqual("256x256", ENameCase::IgnoreCase))
	{
		return EAppImageSize::x256;
	}
	if (Size.IsEqual("512x512", ENameCase::IgnoreCase))
	{
		return EAppImageSize::x512;
	}
	if (Size.IsEqual("1024x1024", ENameCase::IgnoreCase))
	{
		return EAppImageSize::x1024;
	}

	return EAppImageSize::x256;
}

const FName UAppHelper::FormatToName(const EAppResponseFormat Format)
{
	switch (Format)
	{
	case EAppResponseFormat::url:
		return "url";

	case EAppResponseFormat::b64_json:
		return "b64_json";

	default:
		break;
	}

	return NAME_None;
}

const EAppResponseFormat UAppHelper::NameToFormat(const FName Format)
{
	if (Format.IsEqual("url", ENameCase::IgnoreCase))
	{
		return EAppResponseFormat::url;
	}
	if (Format.IsEqual("b64_json", ENameCase::IgnoreCase))
	{
		return EAppResponseFormat::b64_json;
	}

	return EAppResponseFormat::url;
}
