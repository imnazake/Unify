// Copyright Frogster Games. All Rights Reserved.

#include "Input/UnifyInputComponent.h"
#include "Input/UnifyInputConfig.h"
#include "EnhancedInputSubsystems.h"

UUnifyInputComponent::UUnifyInputComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUnifyInputComponent::AddInputMappings(const UUnifyInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	for (const FInputMappingContextAndPriority& Entry: InputConfig->GetInputMappingContexts())
	{
		if (Entry.InputMapping)
		{
			InputSubsystem->AddMappingContext(Entry.InputMapping, Entry.Priority);
		}
	}

	// Here you can handle any custom logic to add something from your input config if required
}

void UUnifyInputComponent::RemoveInputMappings(const UUnifyInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
	
	for (const FInputMappingContextAndPriority& Entry: InputConfig->GetInputMappingContexts())
	{
		if (Entry.InputMapping)
		{
			InputSubsystem->RemoveMappingContext(Entry.InputMapping);
		}
	}

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UUnifyInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (const uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	
	BindHandles.Reset();
}
