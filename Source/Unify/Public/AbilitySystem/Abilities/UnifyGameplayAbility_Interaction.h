// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/UnifyGameplayAbility.h"
#include "UnifyGameplayAbility_Interaction.generated.h"

class UGameplayInteractionOption;
class UGameplayInteractionComponent;

/** Tracks temporary effects and loose tags granted by an interaction phase so they can be revoked cleanly later. */
USTRUCT(BlueprintType)
struct UNIFY_API FUnifyGameplayInteractionGrantResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FActiveGameplayEffectHandle> EffectHandles;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer GrantedTags;

	void Reset()
	{
		EffectHandles.Reset();
		GrantedTags.Reset();
	}
};

/**
 * Project-local interaction ability implementation for Unify.
 *
 * Keep production interaction abilities in your own project module like this one. If you start from the plugin
 * reference ability, recreate the class in your game module or copy the code into your own ability type so you
 * avoid circular dependencies between project code and the plugin.
 */
UCLASS(BlueprintType, Blueprintable)
class UNIFY_API UUnifyGameplayAbility_Interaction : public UUnifyGameplayAbility
{
	GENERATED_BODY()

public:

	UUnifyGameplayAbility_Interaction();

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure, Category = "Ability")
	bool CanExecuteInteraction() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	bool HasValidInteractionData() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	APlayerController* GetPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UGameplayInteractionComponent* GetPlayerInteractionComponent() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	AActor* GetEntityInView() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	AActor* GetCachedEntityFromInteraction() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UActorComponent* GetEntityComponentInView() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UActorComponent* GetCachedEntityComponentInView() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UGameplayInteractionOption* GetCurrentOption() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	AActor* GetCurrentEntity() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UActorComponent* GetCurrentComponent() const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Start Interaction"), Category = "Interaction")
	void BP_OnStartInteraction();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Finish Interaction"), Category = "Interaction")
	void BP_OnFinishInteraction();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Cancel Interaction"), Category = "Interaction")
	void BP_OnCancelInteraction();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Execute Interaction"), Category = "Interaction")
	void BP_OnExecuteInteraction();

protected:

	UFUNCTION()
	virtual void OnInteractionDataReceived(AActor* Entity, UGameplayInteractionOption* Option, FVector_NetQuantizeNormal TargetDirection);

	UFUNCTION()
	virtual void OnInteractionDataCancelled();

	UFUNCTION()
	virtual void OnGameplayEventReceived(FGameplayEventData Payload);

protected:

	void StartInteraction();
	void FinishInteraction();
	void CancelInteraction();

	void StartTimer();
	void ClearTimer();
	void HandleTimedInteractionExecution();

	virtual void OnStartInteraction();
	virtual void OnExecuteInteraction();
	virtual void OnCancelInteraction();
	virtual void OnFinishInteraction();

private:

	FUnifyGameplayInteractionGrantResult GrantInteractionEffectsAndTags(const FGameplayTagContainer& InTags, const TArray<TSubclassOf<UGameplayEffect>>& InEffects) const;
	void RevokeInteractionEffectsAndTags(FUnifyGameplayInteractionGrantResult& InResult) const;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	UGameplayInteractionOption* CurrentOption;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	AActor* CurrentEntity;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	UActorComponent* CurrentComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	FVector CurrentTargetDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	bool bWasExecuted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> ImmobilityEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FScalableFloat ImmobilityEffectLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Validation")
	bool bValidateRemoteClientCameraOrigin = true;

	/**
	 * Enables the server-side facing check for remote clients.
	 *
	 * Leave this disabled if timed interactions should continue after the player looks away.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Validation")
	bool bValidateRemoteClientDirection = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Validation")
	bool bValidateRemoteClientDistance = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Validation")
	bool bValidateRemoteClientAim = true;

private:

	FUnifyGameplayInteractionGrantResult ActivationGrantResult;
	FUnifyGameplayInteractionGrantResult FinishGrantResult;

	FActiveGameplayEffectHandle ImmobilityEffectHandle;
	FTimerHandle TimerHandle;

	friend class UAbilityTask_WaitGameplayEvent;
};
