// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/UnifyGameplayAbility.h"
#include "UnifyGameplayAbility_Interaction_ShowOptions.generated.h"

class UGameplayInteractionComponent;
class APlayerController;

/**
 * Project-local ability for showing or driving multi-option interaction UI.
 *
 * Keep project gameplay abilities in your own module like this one. If you start from the plugin version, recreate
 * the class here or copy its code into your own ability type so the project does not depend on plugin-side ability classes.
 */
UCLASS()
class UNIFY_API UUnifyGameplayAbility_Interaction_ShowOptions : public UUnifyGameplayAbility
{
	GENERATED_BODY()

public:

	UUnifyGameplayAbility_Interaction_ShowOptions();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure, Category = "Ability")
	APlayerController* GetPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UGameplayInteractionComponent* GetPlayerInteractionComponent() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	AActor* GetInteractiveEntityInView() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	AActor* GetCachedInteractiveEntityFromInteraction() const;
};
