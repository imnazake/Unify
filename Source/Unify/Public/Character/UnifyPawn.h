// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "UnifyPawn.generated.h"

class UUnifyAbilitySet;
class UUnifyInputConfig;
class UUnifyInputComponent;
class AUnifyPlayerController;
class UUnifyAbilitySystemComponent;
struct FUnifyAbilitySetGrantedHandles;

UCLASS()
class UNIFY_API AUnifyPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AUnifyPawn();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	virtual void OnRep_PlayerState() override;

	virtual void NotifyControllerChanged() override;
	
	TArray<UUnifyInputConfig*> GetInputConfigs() const;
	TArray<UUnifyAbilitySet*> GetAbilitySets() const;

	UFUNCTION(BlueprintPure, Category = "Unify|Character")
	AUnifyPlayerController* GetUnifyPlayerController() const;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	TArray<FUnifyAbilitySetGrantedHandles*> GetAbilitySetGrantedHandles() const;

	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface

	UFUNCTION(BlueprintPure, Category = "Unify|Character")
	UUnifyAbilitySystemComponent* GetUnifyAbilitySystemComponent() const;

	virtual void InitializePlayerInput(UUnifyInputComponent* PlayerInputComponent);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unify|Input")
	TArray<TObjectPtr<UUnifyInputConfig>> InputConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unify|Abilities")
	TArray<TObjectPtr<UUnifyAbilitySet>> AbilitySets;

private:
	
	TArray<FUnifyAbilitySetGrantedHandles*> AbilitySetsGrantedHandles;
	
};
