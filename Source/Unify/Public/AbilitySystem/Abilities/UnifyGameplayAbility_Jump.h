// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnifyGameplayAbility.h"
#include "UnifyGameplayAbility_Jump.generated.h"

/**
 * UUnifyGameplayAbility_Jump
 */
UCLASS()
class UNIFY_API UUnifyGameplayAbility_Jump : public UUnifyGameplayAbility
{
	GENERATED_BODY()

public:

	UUnifyGameplayAbility_Jump();

protected:
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Unify|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Unify|Ability")
	void CharacterJumpStop();
	
};
