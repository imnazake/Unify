// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnifyLogging.h"
#include "UnifyInputConfig.h"
#include "EnhancedInputComponent.h"
#include "UnifyInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

UCLASS(Config = "Input")
class UNIFY_API UUnifyInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	
	UUnifyInputComponent(const FObjectInitializer& ObjectInitializer);

	void AddInputMappings(const UUnifyInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappings(const UUnifyInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveBinds(TArray<uint32>& BindHandles);

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UUnifyInputConfig* InputConfig, const FGameplayTag& InputTag, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UUnifyInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UUnifyInputComponent::BindNativeAction(const UUnifyInputConfig* InputConfig, const FGameplayTag& InputTag, UserClass* Object, FuncType Func, const bool bLogIfNotFound)
{
	check(InputConfig);

	const FUnifyInputAction Action = InputConfig->FindNativeActionForTag(InputTag, bLogIfNotFound);
	
	if (Action.IsValid())
	{
		for (const ETriggerEvent& TriggerEvent: Action.PressedTriggers)
		{
			if (TriggerEvent != ETriggerEvent::None)
			{
				BindAction(Action.InputAction, TriggerEvent, Object, Func);
			}
		}
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UUnifyInputComponent::BindAbilityActions(const UUnifyInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FUnifyInputAction& Action : InputConfig->GetAbilityInputActions())
	{
		if (Action.IsValid() && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				for (const ETriggerEvent& TriggerEvent: Action.PressedTriggers)
				{
					if (TriggerEvent != ETriggerEvent::None)
					{
						BindHandles.Add(BindAction(Action.InputAction, TriggerEvent, Object, PressedFunc, Action.InputTag).GetHandle());
					}
				}
			}

			if (ReleasedFunc)
			{
				for (const ETriggerEvent& TriggerEvent: Action.ReleasedTriggers)
				{
					if (TriggerEvent != ETriggerEvent::None)
					{
						BindHandles.Add(BindAction(Action.InputAction, TriggerEvent, Object, ReleasedFunc, Action.InputTag).GetHandle());
					}
				}
			}
		}
	}
}
