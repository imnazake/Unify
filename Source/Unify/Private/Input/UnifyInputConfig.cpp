// Copyright Frogster Games. All Rights Reserved.

#include "Input/UnifyInputConfig.h"
#include "UnifyLogging.h"

FInputMappingContextAndPriority::FInputMappingContextAndPriority() : InputMapping(nullptr), Priority(0)
{
}

FUnifyInputAction::FUnifyInputAction()
{
	PressedTriggers.AddUnique(ETriggerEvent::Triggered);
	ReleasedTriggers.AddUnique(ETriggerEvent::Completed);
}

bool FUnifyInputAction::IsValid() const
{
	return InputAction != nullptr;
}

UUnifyInputConfig::UUnifyInputConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FUnifyInputAction UUnifyInputConfig::FindNativeActionForTag(const FGameplayTag& InputTag, const bool bLogNotFound) const
{
	for (const FUnifyInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action;
		}
	}

	if (bLogNotFound)
	{
		BASTION_LOG(Error, TEXT("Can't find NativeAction for InputTag {0} on InputConfig {1}"), *InputTag.ToString(), *THIS_OBJECT_NAME);
	}

	return FUnifyInputAction();
}

const UInputAction* UUnifyInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, const bool bLogNotFound) const
{
	for (const FUnifyInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		BASTION_LOG(Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *THIS_OBJECT_NAME);
	}

	return nullptr;
}

FUnifyInputAction UUnifyInputConfig::FindAbilityActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FUnifyInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action;
		}
	}

	if (bLogNotFound)
	{
		BASTION_LOG(Error, TEXT("Can't find NativeAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *THIS_OBJECT_NAME);
	}

	return FUnifyInputAction();
}

const UInputAction* UUnifyInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, const bool bLogNotFound) const
{
	for (const FUnifyInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		BASTION_LOG(Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *THIS_OBJECT_NAME);
	}

	return nullptr;
}

const TArray<FUnifyInputAction>& UUnifyInputConfig::GetNativeInputActions() const
{
	return NativeInputActions;
}

const TArray<FUnifyInputAction>& UUnifyInputConfig::GetAbilityInputActions() const
{
	return AbilityInputActions;
}

const TArray<FInputMappingContextAndPriority>& UUnifyInputConfig::GetInputMappingContexts() const
{
	return InputMappings;
}
