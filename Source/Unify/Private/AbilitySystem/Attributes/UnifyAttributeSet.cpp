// Copyright Frogster Games. All Rights Reserved.

#include "AbilitySystem/Attributes/UnifyAttributeSet.h"
#include "AbilitySystem/UnifyAbilitySystemComponent.h"

UUnifyAttributeSet::UUnifyAttributeSet()
{
}

UWorld* UUnifyAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UUnifyAbilitySystemComponent* UUnifyAttributeSet::GetUnifyAbilitySystem() const
{
	return Cast<UUnifyAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
