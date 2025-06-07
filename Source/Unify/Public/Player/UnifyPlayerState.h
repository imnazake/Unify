// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "UnifyPlayerState.generated.h"

class UUnifyAbilitySystemComponent;

/**
 * AUnifyPlayerState
 */
UCLASS()
class UNIFY_API AUnifyPlayerState : public APlayerState, public IAbilitySystemInterface 
{
	GENERATED_BODY()

public:

	AUnifyPlayerState();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface

	UFUNCTION(BlueprintPure, Category = "Unify|PlayerState")
	UUnifyAbilitySystemComponent* GetUnifyAbilitySystemComponent() const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Unify|PlayerState")
	TObjectPtr<UUnifyAbilitySystemComponent> AbilitySystemComponent;
	
};
