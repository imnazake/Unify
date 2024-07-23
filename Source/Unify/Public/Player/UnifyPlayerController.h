// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "AbilitySystem/UnifyAbilitySet.h"
#include "UnifyPlayerController.generated.h"

class UGameplayInteractionComponent;
class UHotbarComponent;
class UInventoryComponent;
class UUnifyInputConfig;
class UUnifyHUDLayout;
class UInputMappingContext;
class UUnifyAbilitySystemComponent;

/**
 * AUnifyPlayerController
 */
UCLASS()
class UNIFY_API AUnifyPlayerController : public ACommonPlayerController, public IAbilitySystemInterface
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

private:

	UFUNCTION()
	void OnPossessedPawnChangedCallback(APawn* OldPossessedPawn, APawn* NewPossessedPawn);
	
};
