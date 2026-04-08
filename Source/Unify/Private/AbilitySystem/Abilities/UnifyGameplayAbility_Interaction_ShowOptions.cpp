// Copyright Nazake. All Rights Reserved.

#include "AbilitySystem/Abilities/UnifyGameplayAbility_Interaction_ShowOptions.h"
#include "Core/GameplayEntityInteractionComponent.h"
#include "Core/IGameplayInteractionInterface.h"
#include "Core/GameplayInteractionComponent.h"
#include "Core/IGameplayInteractiveEntityInterface.h"

UUnifyGameplayAbility_Interaction_ShowOptions::UUnifyGameplayAbility_Interaction_ShowOptions()
{
}

bool UUnifyGameplayAbility_Interaction_ShowOptions::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (!ActorInfo)
	{
		return false;
	}

	IGameplayInteractionInterface* InteractionInterface = Cast<IGameplayInteractionInterface>(GetPlayerControllerFromActorInfo());
	if (!InteractionInterface)
	{
		return false;
	}

	UGameplayInteractionComponent* PlayerInteractionComponent = InteractionInterface->GetInteractionComponent();
	if (!PlayerInteractionComponent)
	{
		return false;
	}

	AActor* EntityInView = PlayerInteractionComponent->GetEntityInView();
	if (!EntityInView)
	{
		return false;
	}

	IGameplayInteractiveEntityInterface* EntityInterface = Cast<IGameplayInteractiveEntityInterface>(EntityInView);
	if (!EntityInterface || !EntityInterface->GetEntityInteractionComponent())
	{
		return false;
	}

	bResult &= (PlayerInteractionComponent->GetNumberOfAvailableOptions() > 1);
	return bResult;
}

void UUnifyGameplayAbility_Interaction_ShowOptions::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UUnifyGameplayAbility_Interaction_ShowOptions::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

APlayerController* UUnifyGameplayAbility_Interaction_ShowOptions::GetPlayerControllerFromActorInfo() const
{
	return GetActorInfo().PlayerController.Get();
}

UGameplayInteractionComponent* UUnifyGameplayAbility_Interaction_ShowOptions::GetPlayerInteractionComponent() const
{
	if (IGameplayInteractionInterface* InteractionInterface = Cast<IGameplayInteractionInterface>(GetControllerFromActorInfo()))
	{
		return InteractionInterface->GetInteractionComponent();
	}

	return nullptr;
}

AActor* UUnifyGameplayAbility_Interaction_ShowOptions::GetInteractiveEntityInView() const
{
	if (UGameplayInteractionComponent* PlayerInteractionComponent = GetPlayerInteractionComponent())
	{
		return PlayerInteractionComponent->GetEntityInView();
	}

	return nullptr;
}

AActor* UUnifyGameplayAbility_Interaction_ShowOptions::GetCachedInteractiveEntityFromInteraction() const
{
	if (const UGameplayInteractionComponent* PlayerInteractionComponent = GetPlayerInteractionComponent())
	{
		return PlayerInteractionComponent->GetCachedEntityFromInteraction();
	}

	return nullptr;
}
