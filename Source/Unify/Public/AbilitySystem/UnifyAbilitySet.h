// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "UnifyAbilitySet.generated.h"

class UUnifyAbilitySystemComponent;
class UUnifyGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

/**
 * FUnifyGameplayAbilitySet
 *
 * Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FUnifyGameplayAbilitySet
{
	GENERATED_BODY()

	FUnifyGameplayAbilitySet();
	
	/** Gameplay ability to grant. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUnifyGameplayAbility> AbilityClass;

	/** Level of ability to grant. */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel;

	/** Tag used to process input for the ability. */
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Input"))
	FGameplayTag InputTag;
};

/**
 * FUnifyGameplayEffectSet
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FUnifyGameplayEffectSet
{
	GENERATED_BODY()

	FUnifyGameplayEffectSet();

	/** Gameplay effect to grant. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	/** Level of gameplay effect to grant. */
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel;
};

/**
 * FUnifyGameplayAttributeSet
 *
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FUnifyGameplayAttributeSet
{
	GENERATED_BODY()

	FUnifyGameplayAttributeSet();
	
	/** Gameplay effect to grant. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

};

/**
 * FUnifyAbilitySetGrantedHandles
 */
USTRUCT(BlueprintType)
struct FUnifyAbilitySetGrantedHandles
{
	GENERATED_BODY()
	
	FUnifyAbilitySetGrantedHandles();
	
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void RevokeFromAbilitySystem(UUnifyAbilitySystemComponent* AbilitySystem);

protected:

	/** Handles to the granted abilities. */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	/** Handles to the granted gameplay effects. */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> EffectHandles;

	/** Pointers to the granted attribute sets */
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> AttributeSets;
	
};

/**
 * UUnifyAbilitySet
 *
 * Non-mutable data asset used to grant gameplay abilities, effects and attribute sets.
 */
UCLASS(BlueprintType, Blueprintable, Const)
class UNIFY_API UUnifyAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UUnifyAbilitySet();

	void GrantToAbilitySystem(UUnifyAbilitySystemComponent* AbilitySystem, FUnifyAbilitySetGrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

protected:

	/** Gameplay abilities to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FUnifyGameplayAbilitySet> GrantedAbilities;

	/** Gameplay effects to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<FUnifyGameplayEffectSet> GrantedEffects;

	/** Attribute sets to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TArray<FUnifyGameplayAttributeSet> GrantedAttributes;
	
};
