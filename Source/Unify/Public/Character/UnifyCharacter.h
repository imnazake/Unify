// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Unify/Unify.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"

/** Uncomment if you are using GameplayContainers */
//#include "Core/Equipment/IGameplayEquipmentInterface.h"
//#include "Core/IGameplayContainerInterface.h"

//class UHotbarComponent;
//class UPawnEquipmentComponent;
/** Uncomment if you are using GameplayContainers */

/** Uncomment if you are using GameplayInteraction */
#include "Core/IGameplayInteractionInterface.h"
/** Uncomment if you are using GameplayInteraction */

#include "GameFramework/Character.h"
#include "UnifyCharacter.generated.h"

class UCameraComponent;
class UUnifyAbilitySet;
class USpringArmComponent;
class UUnifyInputConfig;
class UUnifyInputComponent;
class USkeletalMeshComponent;
class AUnifyPlayerController;
class UUnifyAbilitySystemComponent;
struct FUnifyAbilitySetGrantedHandles;

/**
 * AUnifyCharacter
 */
UCLASS()
class UNIFY_API AUnifyCharacter : public ACharacter, public IAbilitySystemInterface

/** Uncomment if you are using GameplayContainers */
	//, public IGameplayContainerInterface
	//, public IGameplayEquipmentInterface
/** Uncomment if you are using GameplayContainers */

/** Uncomment if you are using GameplayInteraction */
	, public IGameplayInteractionInterface
/** Uncomment if you are using GameplayInteraction */

{
	GENERATED_BODY()

public:

	AUnifyCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void NotifyControllerChanged() override;

	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	TArray<UUnifyInputConfig*> GetInputConfigs() const;
	TArray<UUnifyAbilitySet*> GetAbilitySets() const;

	AUnifyPlayerController* GetUnifyPlayerController() const;
	
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	/** Uncomment if you are using GameplayInteraction */
	UFUNCTION(BlueprintPure, Category = "Unify|Character")
	virtual UGameplayInteractionComponent* GetInteractionComponent() override;
	/** Uncomment if you are using GameplayInteraction */

	/** Uncomment if you are using GameplayContainers */
	void Input_Hotbar_1();
	void Input_Hotbar_2();
	void Input_Hotbar_3();
	void Input_Hotbar_4();
	void Input_Hotbar_5();
	void Input_Hotbar_6();
	/** Uncomment if you are using GameplayContainers */

	/** Uncomment if you are using GameplayInteraction */
	//UFUNCTION(BlueprintPure, Category = "Unify|Character")
	//class UGameplayInteractionComponent* GetInteractionComponent();

	void EnableMovementAndCollision() const;
	void DisableMovementAndCollision() const;

	TArray<FUnifyAbilitySetGrantedHandles*> GetAbilitySetGrantedHandles() const;

	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface

	/** Uncomment if you are using GameplayContainers */
	//virtual TArray<UGameplayContainerComponent*> GetAllContainers() override;
	//virtual UInventoryComponent* GetInventoryComponent() override;
	//virtual UHotbarComponent* GetHotbarComponent() override;
	//virtual UEquipmentComponent* GetEquipmentComponent() override;
	/** Uncomment if you are using GameplayContainers */
	
	/** Uncomment if you are using GameplayContainers */
	//UFUNCTION(BlueprintPure, Category = "Unify|PlayerController")
	//virtual UGameplayContainerComponent* GetActiveContainerComponent() override;
	/** Uncomment if you are using GameplayContainers */
	
	//virtual UMeshComponent* GetMeshComponentByTag(FName ComponentTag) const override;
	//virtual void OnItemEquipped(const UGameplayItemDefinition* Item) override;
	//virtual void OnItemUnequipped(const UGameplayItemDefinition* Item) override;

	UFUNCTION(BlueprintPure, Category = "Unify|Character")
	UUnifyAbilitySystemComponent* GetUnifyAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintPure, Category = "Unify|Character")
	UCameraComponent* GetFirstPersonCamera() const;

	virtual void InitializePlayerInput(UUnifyInputComponent* PlayerInputComponent);

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

	/** Uncomment if you are using GameplayContainers */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|Character")
	//TObjectPtr<UHotbarComponent> HotbarComponent;
	/** Uncomment if you are using GameplayContainers */

	/** Uncomment if you are using GameplayContainers */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Unify|Character")
	//TObjectPtr<UPawnEquipmentComponent> EquipmentComponent;
	/** Uncomment if you are using GameplayContainers */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Torso;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Legs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Hands;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Feet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Camera")
	TObjectPtr<USpringArmComponent> ThirdPersonCameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Camera")
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unify|Input")
	TArray<TObjectPtr<UUnifyInputConfig>> InputConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unify|Abilities")
	TArray<TObjectPtr<UUnifyAbilitySet>> AbilitySets;

private:
	
	TArray<FUnifyAbilitySetGrantedHandles*> AbilitySetsGrantedHandles;
	
};
