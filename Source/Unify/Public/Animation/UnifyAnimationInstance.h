// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "UnifyAnimationInstance.generated.h"

class UAbilitySystemComponent;

/**
 * UUnifyAnimationInstance
 */
UCLASS()
class UNIFY_API UUnifyAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UUnifyAnimationInstance();

	virtual void RegisterWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	// Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags are added or removed.
	// These should be used instead of manually querying for the gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayTags")
	float GroundDistance;
	
};
