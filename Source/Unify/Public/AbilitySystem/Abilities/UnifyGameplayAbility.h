// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Abilities/GameplayAbility.h>
#include "UnifyGameplayAbility.generated.h"

class UUnifyAbilitySystemComponent;
class AUnifyPlayerController;
class AUnifyCharacter;

/**
 * EGameplayAbilityActivationPolicy
 *
 * Defines different activation policies for abilities in Bastion.
 */
UENUM(BlueprintType)
enum class EGameplayAbilityActivationPolicy : uint8
{
	// Activates the ability upon a single input press event.
	OnInputPressed,

	// Activates the ability continuously while the input is held down.
	OnInputHeld,

	// Activates the ability upon the spawning or assigning of an avatar.
	OnSpawn,
};

/**
 * EGameplayAbilityActivationGroup
 *
 * Defines how an ability activates in relation to other abilities.
 */
UENUM(BlueprintType)
enum class EGameplayAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.
	Exclusive_Replaceable,

	// Ability blocks all other exclusive abilities from activating.
	Exclusive_Blocking,
};

/**
 * UUnifyGameplayAbility
 */
UCLASS()
class UNIFY_API UUnifyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UUnifyGameplayAbility();

	UFUNCTION(BlueprintPure, Category = "Unify|Ability")
	EGameplayAbilityActivationPolicy GetActivationPolicy() const;

	UFUNCTION(BlueprintPure, Category = "Unify|Ability")
	EGameplayAbilityActivationGroup GetActivationGroup() const;

	UFUNCTION(BlueprintCallable, Category = "Unify|Ability")
	UUnifyAbilitySystemComponent* GetUnifyAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Unify|Ability")
	AUnifyPlayerController* GetUnifyPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Unify|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Unify|Ability")
	AUnifyCharacter* GetUnifyCharacterFromActorInfo() const;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	//virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const ILyraAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnPawnAvatarSet();

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Ability Granted", Category = "Ability")
	void BP_OnAbilityGranted();

	/** Called when this ability is revoked from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Ability Revoked", Category = "Ability")
	void BP_OnAbilityRevoked();

	/** Called when the ability system is initialized with an avatar. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Avatar Set", Category = "Ability")
	void BP_OnAvatarSet();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Activation")
	EGameplayAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Activation")
	EGameplayAbilityActivationGroup ActivationGroup;
	
};
