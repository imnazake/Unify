// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "AbilitySystem/UnifyAbilitySet.h"

/** Uncomment if you are using GameplayContainers */
//#include "Core/IGameplayContainerInterface.h"
//#include "Core/Users/IGameplayContainerUserInterface.h"

//class UInventoryComponent;
/** Uncomment if you are using GameplayContainers */

/** Uncomment if you are using GameplayInteraction */
#include "Core/IGameplayInteractionInterface.h"
class UGameplayInteractionComponent;
/** Uncomment if you are using GameplayInteraction */

#include "UnifyPlayerController.generated.h"


class UUnifyInputConfig;
class UUnifyHUDLayout;
class UInputMappingContext;
class UUnifyAbilitySystemComponent;

/**
 * AUnifyPlayerController
 */
UCLASS()
class UNIFY_API AUnifyPlayerController : public ACommonPlayerController, public IAbilitySystemInterface
/** Uncomment if you are using GameplayContainers */
	//, public IGameplayContainerInterface
	//, public IGameplayContainerUserInterface

/** Uncomment if you are using GameplayInteraction */
	, public IGameplayInteractionInterface
{
	GENERATED_BODY()

public:

	AUnifyPlayerController();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

	/**
	 * Enables cheats for the class.
	 */
	virtual void EnableCheats() override;

	/**
	 * Adds cheat commands.
	 *
	 * @param bForce If true, force the addition of cheats.
	 */
	virtual void AddCheats(bool bForce) override;
	
	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void AcknowledgePossession(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintPure, Category = "Unify|PlayerController")
	TArray<UUnifyInputConfig*> GetInputConfigsForPawn(APawn* InPawn) const;
	
	UFUNCTION(BlueprintCallable, Category = "Unify|PlayerController")
	UUnifyAbilitySystemComponent* GetUnifyAbilitySystemComponent() const;

	/** Uncomment if you are using GameplayContainers */
	//virtual TArray<UGameplayContainerComponent*> GetAllContainers() override;
	//virtual UInventoryComponent* GetInventoryComponent() override;
	//virtual UHotbarComponent* GetHotbarComponent() override;
	//virtual UEquipmentComponent* GetEquipmentComponent() override;
	/** Uncomment if you are using GameplayContainers */
	
	/** Uncomment if you are using GameplayContainers */
	//virtual void OnContainerUserRegistered(const FGameplayContainerUser& User) override;
	//virtual void OnContainerUserUnregistered(const FGameplayContainerUser& User) override;
	//virtual void OnContainerUserInfoChanged(const FGameplayContainerUser& User) override;
	/** Uncomment if you are using GameplayContainers */

	/** Uncomment if you are using GameplayContainers */
	//UFUNCTION(BlueprintPure, Category = "Unify|PlayerController")
	//virtual UGameplayContainerComponent* GetActiveContainerComponent() override;
	/** Uncomment if you are using GameplayContainers */
	
	/** Uncomment if you are using GameplayContainers */
	//UFUNCTION(BlueprintPure, Category = "Unify|Character")
	//UInventoryComponent* GetInventoryComponent() const;
	/** Uncomment if you are using GameplayContainers */

	
	/** Uncomment if you are using GameplayInteraction */
	UFUNCTION(BlueprintPure, Category = "Unify|PlayerController")
	virtual UGameplayInteractionComponent* GetInteractionComponent() override;

protected:
	
	/** Uncomment if you are using GameplayContainers */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|PlayerController")
	//TObjectPtr<UInventoryComponent> InventoryComponent;
	
	/** Uncomment if you are using GameplayContainers */
	//UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|PlayerController")
	//TObjectPtr<UGameplayContainerComponent> ActiveContainer;
	
	/** Uncomment if you are using GameplayInteraction */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|PlayerController")
	TObjectPtr<UGameplayInteractionComponent> InteractionComponent;

private:

	UFUNCTION()
	void OnPossessedPawnChangedCallback(APawn* OldPossessedPawn, APawn* NewPossessedPawn);
	
};
