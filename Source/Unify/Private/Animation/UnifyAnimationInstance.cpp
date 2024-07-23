// Copyright Frogster Games. All Rights Reserved.

#include "Animation/UnifyAnimationInstance.h"
#include "Character/UnifyCharacterMovementComponent.h"
#include "Character/UnifyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UUnifyAnimationInstance::UUnifyAnimationInstance() : GroundDistance(-1.0f)
{
}

void UUnifyAnimationInstance::RegisterWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);
	GameplayTagPropertyMap.Initialize(this, AbilitySystem);
}

void UUnifyAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningActor()))
	{
		RegisterWithAbilitySystem(ASC);
	}
}

void UUnifyAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const AUnifyCharacter* Character = Cast<AUnifyCharacter>(GetOwningActor()))
	{
		UUnifyCharacterMovementComponent* MovementComponent = CastChecked<UUnifyCharacterMovementComponent>(Character->GetMovementComponent());
		GroundDistance = MovementComponent->GetGroundInfo().GroundDistance;
	}
}
