// Copyright Nazake. All Rights Reserved.

#include "Player/UnifyPlayerController.h"
#include "Player/UnifyPlayerState.h"
#include "AbilitySystem/UnifyAbilitySystemComponent.h"
#include "Input/UnifyInputComponent.h"
#include "Input/UnifyInputConfig.h"
#include "Character/UnifyCharacter.h"
#include "Character/UnifyPawn.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemGlobals.h"
#include "UnifyLogging.h"

/** Uncomment if you are using GameplayContainers */
//#include "Core/Inventory/InventoryComponent.h"
//#include "Core/Equipment/EquipmentComponent.h"
//#include "Core/Hotbar/HotbarComponent.h"
/** Uncomment if you are using GameplayContainers */

/** Uncomment if you are using GameplayContainers */
#include "Core/GameplayInteractionComponent.h"
/** Uncomment if you are using GameplayContainers */

AUnifyPlayerController::AUnifyPlayerController()
{
	// Required for aiming with the player controller and using its transform
	bAttachToPawn = true;

	/** Uncomment if you are using GameplayContainers */
	//InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	//InventoryComponent->SetIsReplicated(true);
	/** Uncomment if you are using GameplayContainers */

	/** Uncomment if you are using GameplayInteraction */
	InteractionComponent = CreateDefaultSubobject<UGameplayInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetIsReplicated(true);
	/** Uncomment if you are using GameplayInteraction */

}

void AUnifyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AUnifyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AUnifyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChangedCallback);
}

void AUnifyPlayerController::EnableCheats()
{
	Super::EnableCheats();
}

void AUnifyPlayerController::AddCheats(const bool bForce)
{
	Super::AddCheats(/*bForce*/ true);
}

UAbilitySystemComponent* AUnifyPlayerController::GetAbilitySystemComponent() const
{
	if (const AUnifyPlayerState* MyPlayerState = GetPlayerState<AUnifyPlayerState>())
	{
		return MyPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

/** Uncomment if you are using GameplayInteraction */

UGameplayInteractionComponent* AUnifyPlayerController::GetInteractionComponent()
{
	return InteractionComponent;
}

/** Uncomment if you are using GameplayInteraction */

void AUnifyPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AUnifyPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UUnifyAbilitySystemComponent* ASC = GetUnifyAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UUnifyAbilitySystemComponent* AUnifyPlayerController::GetUnifyAbilitySystemComponent() const
{
	if (const AUnifyPlayerState* MyPlayerState = GetPlayerState<AUnifyPlayerState>())
	{
		return MyPlayerState->GetUnifyAbilitySystemComponent();
	}

	return nullptr;
}

/** Uncomment if you are using GameplayContainers */

/*TArray<UGameplayContainerComponent*> AUnifyPlayerController::GetAllContainers()
{
	TArray<UGameplayContainerComponent*> FoundContainers;

	FoundContainers.AddUnique(InventoryComponent);
	
	return FoundContainers;
}

UInventoryComponent* AUnifyPlayerController::GetInventoryComponent()
{
	return InventoryComponent;
}

UHotbarComponent* AUnifyPlayerController::GetHotbarComponent()
{
	if (AUnifyCharacter* MyCharacter = Cast<AUnifyCharacter>(GetPawn()))
	{
		return MyCharacter->GetHotbarComponent();
	}

	return nullptr;
}

UEquipmentComponent* AUnifyPlayerController::GetEquipmentComponent()
{
	if (AUnifyCharacter* MyCharacter = Cast<AUnifyCharacter>(GetPawn()))
	{
		return MyCharacter->GetEquipmentComponent();
	}

	return nullptr;
}

void AUnifyPlayerController::OnContainerUserRegistered(const FGameplayContainerUser& User)
{
	if (const APlayerController* PC = User.GetAbilitySystemComponent()->AbilityActorInfo.Get()->PlayerController.Get())
	{
		if (PC == this)
		{
			ActiveContainer = User.GetTargetContainer();
		}
	}
}

void AUnifyPlayerController::OnContainerUserUnregistered(const FGameplayContainerUser& User)
{
	if (const APlayerController* PC = User.GetAbilitySystemComponent()->AbilityActorInfo.Get()->PlayerController.Get())
	{
		if (PC == this)
		{
			ActiveContainer = nullptr;
		}
	}
}

void AUnifyPlayerController::OnContainerUserInfoChanged(const FGameplayContainerUser& User)
{
	if (const APlayerController* PC = User.GetAbilitySystemComponent()->AbilityActorInfo.Get()->PlayerController.Get())
	{
		if (PC == this)
		{
			ActiveContainer = User.GetTargetContainer();
		}
	}
}

UGameplayContainerComponent* AUnifyPlayerController::GetActiveContainerComponent()
{
	return ActiveContainer;
}

UInventoryComponent* AUnifyPlayerController::GetInventoryComponent() const
{
	return InventoryComponent;
}*/

/** Uncomment if you are using GameplayContainers */

void AUnifyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	/** Uncomment if you are using GameplayContainers */
	/*if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		InventoryComponent->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
	}*/
	/** Uncomment if you are using GameplayContainers */

	/** Uncomment if you are using GameplayInteraction */
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		InteractionComponent->RegisterAbilitySystemComponent(PS->GetAbilitySystemComponent());
	}
	/** Uncomment if you are using GameplayInteraction */
	
}

void AUnifyPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unprocessed doesn't remain our ASC's avatar actor
	// (e.g., driving vehicles or riding mounts and stuff like that while the asc is on the player state but using the possessed pawn as avatar)
	
	if (const APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}
	
	Super::OnUnPossess();
}

void AUnifyPlayerController::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);
}

void AUnifyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	/** Uncomment if you are using GameplayContainers */
	/*if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		InventoryComponent->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
	}*/
	/** Uncomment if you are using GameplayContainers */

	/** Uncomment if you are using GameplayInteraction */
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		InteractionComponent->RegisterAbilitySystemComponent(PS->GetAbilitySystemComponent());
	}
	/** Uncomment if you are using GameplayInteraction */
	
}

void AUnifyPlayerController::OnPossessedPawnChangedCallback(APawn* OldPossessedPawn, APawn* NewPossessedPawn)
{
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			const UUnifyInputComponent* MyInputComponent = Cast<UUnifyInputComponent>(InputComponent);
			check(MyInputComponent);

			if (OldPossessedPawn)
			{
				const TArray<UUnifyInputConfig*> OldInputConfigs = GetInputConfigsForPawn(OldPossessedPawn);

				for (const UUnifyInputConfig* OldInputConfig: OldInputConfigs)
				{
					MyInputComponent->RemoveInputMappings(OldInputConfig, InputSubsystem);
				}
			}

			if (NewPossessedPawn)
			{
				const TArray<UUnifyInputConfig*> NewInputConfigs = GetInputConfigsForPawn(NewPossessedPawn);
				
				for (const UUnifyInputConfig* NewInputConfig: NewInputConfigs)
				{
					MyInputComponent->AddInputMappings(NewInputConfig, InputSubsystem);
				}
			}
		}
	}
}

TArray<UUnifyInputConfig*> AUnifyPlayerController::GetInputConfigsForPawn(APawn* InPawn) const
{
	if (const AUnifyPawn* MyPawn = Cast<AUnifyPawn>(InPawn))
	{
		return MyPawn->GetInputConfigs();
	}
	
	if (const AUnifyCharacter* MyCharacter = Cast<AUnifyCharacter>(InPawn))
	{
		return MyCharacter->GetInputConfigs();
	}

	UNIFY_LOG(Warning, TEXT("%s - Pawn (%s) does not have an input config! It needs to inherit from UnifyPawn or UnifyCharacter classes."), *THIS_FUNCTION, *GetNameSafe(InPawn));
	return TArray<UUnifyInputConfig*>();
}
