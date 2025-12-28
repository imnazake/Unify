// Copyright Nazake. All Rights Reserved.

#include "AbilitySystem/Abilities/UnifyGameplayAbility.h"
#include "AbilitySystem/UnifyAbilitySystemComponent.h"
#include "Player/UnifyPlayerController.h"
#include "Character/UnifyCharacter.h"

UUnifyGameplayAbility::UUnifyGameplayAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	ActivationPolicy = EGameplayAbilityActivationPolicy::OnInputPressed;
	ActivationGroup = EGameplayAbilityActivationGroup::Independent;
}

EGameplayAbilityActivationPolicy UUnifyGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}

EGameplayAbilityActivationGroup UUnifyGameplayAbility::GetActivationGroup() const
{
	return ActivationGroup;
}

UUnifyAbilitySystemComponent* UUnifyGameplayAbility::GetUnifyAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UUnifyAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

AUnifyPlayerController* UUnifyGameplayAbility::GetUnifyPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AUnifyPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* UUnifyGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (const APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

AUnifyCharacter* UUnifyGameplayAbility::GetUnifyCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AUnifyCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

void UUnifyGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	BP_OnAbilityGranted();
	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UUnifyGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	BP_OnAbilityRevoked();
	
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UUnifyGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.GetPrimaryInstance()->GetCurrentActivationInfo().ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == EGameplayAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UUnifyGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	BP_OnAvatarSet();
}

void UUnifyGameplayAbility::OnPawnAvatarSet()
{
	
}
