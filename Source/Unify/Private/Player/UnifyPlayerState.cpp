// Copyright Frogster Games. All Rights Reserved.

#include "Player/UnifyPlayerState.h"
#include "AbilitySystem/UnifyAbilitySystemComponent.h"

AUnifyPlayerState::AUnifyPlayerState()
{
	// Create ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UUnifyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void AUnifyPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AUnifyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

UAbilitySystemComponent* AUnifyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UUnifyAbilitySystemComponent* AUnifyPlayerState::GetUnifyAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
