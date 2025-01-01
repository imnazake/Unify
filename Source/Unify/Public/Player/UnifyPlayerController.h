// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Unify/Unify.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "AbilitySystem/UnifyAbilitySet.h"

#if COMPILE_GAMEPLAY_CONTAINERS
#include "Core/IGameplayContainerInterface.h"
#include "Core/Users/IGameplayContainerUserInterface.h"
#endif

#if COMPILE_GAMEPLAY_INTERACTION
#include "Core/IGameplayInteractionInterface.h"
#endif

#include "UnifyPlayerController.generated.h"

class UHotbarComponent;
class UInventoryComponent;
class UUnifyInputConfig;
class UUnifyHUDLayout;
class UInputMappingContext;
class UUnifyAbilitySystemComponent;
class UGameplayInteractionComponent;

/**
 * AUnifyPlayerController
 */
UCLASS()
class UNIFY_API AUnifyPlayerController : public ACommonPlayerController, public IAbilitySystemInterface

#if COMPILE_GAMEPLAY_CONTAINERS
	, public IGameplayContainerInterface
	, public IGameplayContainerUserInterface
#endif

#if COMPILE_GAMEPLAY_INTERACTION
	, public IGameplayInteractionInterface
#endif

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

//#if COMPILE_GAMEPLAY_CONTAINERS

	//~IGameplayContainerInterface
	virtual TArray<UGameplayContainerComponent*> GetGameplayContainers() override;
	virtual UInventoryComponent* GetInventoryComponent() override;
	virtual UHotbarComponent* GetHotbarComponent() override;
	virtual UEquipmentComponent* GetEquipmentComponent() override;

	UFUNCTION(BlueprintPure, Category = "Unify|PlayerController")
	virtual UGameplayContainerComponent* GetActiveContainerComponent() override;
	//~IGameplayContainerInterface

	//~IGameplayContainerUserInterface
	virtual void OnContainerUserRegistered(const FGameplayContainerUser& User) override;
	virtual void OnContainerUserUnregistered(const FGameplayContainerUser& User) override;
	virtual void OnContainerUserInfoChanged(const FGameplayContainerUser& User) override;
	//~IGameplayContainerUserInterface
	
	void SetActiveContainerComponent(UGameplayContainerComponent* NewContainer);

//#endif

#if COMPILE_GAMEPLAY_CONTAINERS
	/** Uncomment this if compile gameplay containers is enabled. */
	//UFUNCTION(BlueprintPure, Category = "Unify|Character")
	UInventoryComponent* GetInventoryComponent() const;
	
	/** Uncomment this if compile gameplay containers is enabled. */
	//UFUNCTION(BlueprintPure, Category = "Unify|Character")
	UHotbarComponent* GetHotbarComponent() const;
#endif
	
#if COMPILE_GAMEPLAY_INTERACTION
	/** Uncomment this if compile gameplay interaction is enabled. */
	//UFUNCTION(BlueprintPure, Category = "Unify|PlayerController")
	virtual UGameplayInteractionComponent* GetInteractionComponent() override;
#endif

protected:

//#if COMPILE_GAMEPLAY_CONTAINERS
	/** Uncomment this if compile gameplay containers is enabled. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|PlayerController")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Uncomment this if compile gameplay containers is enabled. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|PlayerController")
	TObjectPtr<UHotbarComponent> HotbarComponent;

	/** Uncomment this if compile gameplay containers is enabled. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|PlayerController")
	TObjectPtr<UGameplayContainerComponent> ActiveContainer;
//#endif

#if COMPILE_GAMEPLAY_INTERACTION
	/** Uncomment this if compile gameplay interaction is enabled. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|PlayerController")
	TObjectPtr<UGameplayInteractionComponent> InteractionComponent;
#endif

private:

	UFUNCTION()
	void OnPossessedPawnChangedCallback(APawn* OldPossessedPawn, APawn* NewPossessedPawn);
	
};
