// Copyright Nazake. All Rights Reserved.

#include "AbilitySystem/UnifyAbilitySet.h"
#include "AbilitySystem/UnifyAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/UnifyGameplayAbility.h"
#include "UnifyLogging.h"

FUnifyGameplayAbilitySet::FUnifyGameplayAbilitySet() : AbilityClass(nullptr), AbilityLevel(1), InputTag(FGameplayTag::EmptyTag)
{
}

FUnifyGameplayEffectSet::FUnifyGameplayEffectSet() : EffectClass(nullptr), EffectLevel(1.0f)
{
}

FUnifyGameplayAttributeSet::FUnifyGameplayAttributeSet() : AttributeSet(nullptr)
{
}

FUnifyAbilitySetGrantedHandles::FUnifyAbilitySetGrantedHandles()
{
}

void FUnifyAbilitySetGrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FUnifyAbilitySetGrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		EffectHandles.Add(Handle);
	}
}

void FUnifyAbilitySetGrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	AttributeSets.Add(Set);
}

void FUnifyAbilitySetGrantedHandles::RevokeFromAbilitySystem(UUnifyAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);

	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : EffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : AttributeSets)
	{
		AbilitySystem->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	EffectHandles.Reset();
	AttributeSets.Reset();
}

UUnifyAbilitySet::UUnifyAbilitySet()
{
}

void UUnifyAbilitySet::GrantToAbilitySystem(UUnifyAbilitySystemComponent* AbilitySystem, FUnifyAbilitySetGrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(AbilitySystem);

	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedAbilities.Num(); ++AbilityIndex)
	{
		const FUnifyGameplayAbilitySet& AbilityToGrant = GrantedAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.AbilityClass))
		{
			UNIFY_LOG(Error, TEXT("GrantedAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UUnifyGameplayAbility* AbilityCDO = AbilityToGrant.AbilityClass->GetDefaultObject<UUnifyGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystem->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedEffects.Num(); ++EffectIndex)
	{
		const FUnifyGameplayEffectSet& EffectToGrant = GrantedEffects[EffectIndex];

		if (!IsValid(EffectToGrant.EffectClass))
		{
			UNIFY_LOG(Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.EffectClass->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystem->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, AbilitySystem->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FUnifyGameplayAttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UNIFY_LOG(Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(AbilitySystem->GetOwner(), SetToGrant.AttributeSet);
		AbilitySystem->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}
