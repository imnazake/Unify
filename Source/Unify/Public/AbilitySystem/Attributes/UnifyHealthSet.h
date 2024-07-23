// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "UnifyAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UnifyHealthSet.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);

/**
 * UUnifyHealthSet
 */
UCLASS()
class UNIFY_API UUnifyHealthSet : public UUnifyAttributeSet
{
	GENERATED_BODY()

public:

	UUnifyHealthSet();

	ATTRIBUTE_ACCESSORS(UUnifyHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UUnifyHealthSet, MaxHealth);

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	
	/**
	 * The current health attribute.
	 * The health will be capped by the max health attribute.
	 * Health is hidden from modifiers so only executions can modify it.
	 */
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, meta = (HideFromModifiers, AllowPrivateAccess = true), Category = "Unify|Health")
	FGameplayAttributeData Health;

	/**
	 * The current max health attribute.
	 * Max health is an attribute since gameplay effects can modify it.
	 */
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, meta = (HideFromModifiers, AllowPrivateAccess = true), Category = "Unify|Health")
	FGameplayAttributeData MaxHealth;
	
};
