// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "UnifyCharacter.generated.h"

class UEquipmentComponent;
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
{
	GENERATED_BODY()

public:

	AUnifyCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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

	void Input_Hotbar_1();
	void Input_Hotbar_2();
	void Input_Hotbar_3();
	void Input_Hotbar_4();
	void Input_Hotbar_5();
	void Input_Hotbar_6();

	void EnableMovementAndCollision() const;
	void DisableMovementAndCollision() const;

	TArray<FUnifyAbilitySetGrantedHandles*> GetAbilitySetGrantedHandles() const;

	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface

#if COMPILE_GAMEPLAY_CONTAINERS

	//~IGameplayContainerInterface
	virtual TArray<UGameplayContainerComponent*> GetGameplayContainers() override;
	virtual UInventoryComponent* GetInventoryComponent() override;
	virtual UHotbarComponent* GetHotbarComponent() override;
	virtual UEquipmentComponent* GetEquipmentComponent() override;
	//~IGameplayContainerInterface

#endif

	UFUNCTION(BlueprintPure, Category = "Unify|Character")
	UUnifyAbilitySystemComponent* GetUnifyAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintPure, Category = "Unify|Character")
	UCameraComponent* GetFirstPersonCamera() const;

	virtual void InitializePlayerInput(UUnifyInputComponent* PlayerInputComponent);

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

#if COMPILE_GAMEPLAY_CONTAINERS
	/** Uncomment this if compile gameplay containers is enabled. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Equipment")
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Hands;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Legs;

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
