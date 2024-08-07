// Copyright Nazake. All Rights Reserved.

#include "AbilitySystem/Abilities/UnifyGameplayAbility_Jump.h"
#include "Character/UnifyCharacter.h"

UUnifyGameplayAbility_Jump::UUnifyGameplayAbility_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UUnifyGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const AUnifyCharacter* UnifyCharacter = Cast<AUnifyCharacter>(ActorInfo->AvatarActor.Get());
	if (!UnifyCharacter || !UnifyCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UUnifyGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnifyGameplayAbility_Jump::CharacterJumpStart()
{
	if (AUnifyCharacter* UnifyCharacter = GetUnifyCharacterFromActorInfo())
	{
		if (UnifyCharacter->IsLocallyControlled() && !UnifyCharacter->bPressedJump)
		{
			UnifyCharacter->UnCrouch();
			UnifyCharacter->Jump();
		}
	}
}

void UUnifyGameplayAbility_Jump::CharacterJumpStop()
{
	if (AUnifyCharacter* UnifyCharacter = GetUnifyCharacterFromActorInfo())
	{
		if (UnifyCharacter->IsLocallyControlled() && UnifyCharacter->bPressedJump)
		{
			UnifyCharacter->StopJumping();
		}
	}
}
