// Copyright Frogster Games. All Rights Reserved.

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

	/*//~IGameplayContainerInterface
	virtual TArray<UGameplayContainerComponent*> GetGameplayContainers() override;
	virtual UInventoryComponent* GetInventoryComponent() override;
	virtual UHotbarComponent* GetHotbarComponent() override;
	virtual UEquipmentComponent* GetEquipmentComponent() override;
	//~IGameplayContainerInterface*/

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_Hotbar_1();
	void Input_Hotbar_2();
	void Input_Hotbar_3();
	void Input_Hotbar_4();
	void Input_Hotbar_5();
	void Input_Hotbar_6();

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
