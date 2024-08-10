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

#if COMPILE_GAMEPLAY_CONTAINERS
#include "Core/Inventory/InventoryComponent.h"
#include "Core/Equipment/EquipmentComponent.h"
#include "Core/Hotbar/HotbarComponent.h"
#endif

#if COMPILE_GAMEPLAY_INTERACTION
#include "Core/GameplayInteractionComponent.h"
#endif

AUnifyPlayerController::AUnifyPlayerController()
{
	// Required for aiming with the player controller and using its transform
	bAttachToPawn = true;

#if COMPILE_GAMEPLAY_CONTAINERS
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);
	
	HotbarComponent = CreateDefaultSubobject<UHotbarComponent>(TEXT("HotbarComponent"));
	HotbarComponent->SetIsReplicated(true);
#endif

#if COMPILE_GAMEPLAY_INTERACTION
	InteractionComponent = CreateDefaultSubobject<UGameplayInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetIsReplicated(true);
#endif

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

#if COMPILE_GAMEPLAY_INTERACTION

UGameplayInteractionComponent* AUnifyPlayerController::GetInteractionComponent()
{
	return InteractionComponent;
}

#endif

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

#if COMPILE_GAMEPLAY_CONTAINERS

TArray<UGameplayContainerComponent*> AUnifyPlayerController::GetGameplayContainers()
{
	TArray<UGameplayContainerComponent*> FoundContainers;

	FoundContainers.AddUnique(InventoryComponent);
	FoundContainers.AddUnique(HotbarComponent);
	
	return FoundContainers;
}

UInventoryComponent* AUnifyPlayerController::GetInventoryComponent()
{
	return InventoryComponent;
}

UHotbarComponent* AUnifyPlayerController::GetHotbarComponent()
{
	return HotbarComponent;
}

UEquipmentComponent* AUnifyPlayerController::GetEquipmentComponent()
{
	return Cast<AUnifyCharacter>(GetPawn())->GetEquipmentComponent();
}

#endif

#if COMPILE_GAMEPLAY_CONTAINERS

UInventoryComponent* AUnifyPlayerController::GetInventoryComponent() const
{
	return InventoryComponent;
}

UHotbarComponent* AUnifyPlayerController::GetHotbarComponent() const
{
	return HotbarComponent;
}

#endif

void AUnifyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#if COMPILE_GAMEPLAY_CONTAINERS
	
	if (AUnifyCharacter* MyCharacter = Cast<AUnifyCharacter>(InPawn))
	{
		MyCharacter->GetEquipmentComponent()->RegisterWithInventoryComponent(InventoryComponent);
	}

	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		HotbarComponent->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
		InventoryComponent->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
	}

#endif

#if COMPILE_GAMEPLAY_INTERACTION

	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		InteractionComponent->RegisterAbilitySystemComponent(PS->GetAbilitySystemComponent());
	}
	
#endif
	
}

void AUnifyPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	// (e.g. driving vehicles or riding mounts and stuff like that while the asc is on the player state but using the possessed pawn as avatar)
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

#if COMPILE_GAMEPLAY_CONTAINERS
	
	if (AUnifyCharacter* MyCharacter = Cast<AUnifyCharacter>(InPawn))
	{
		MyCharacter->GetEquipmentComponent()->RegisterWithInventoryComponent(InventoryComponent);
	}

#endif
	
}

void AUnifyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

#if COMPILE_GAMEPLAY_CONTAINERS
	
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		HotbarComponent->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
		InventoryComponent->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
	}

#endif

#if COMPILE_GAMEPLAY_INTERACTION
	
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		InteractionComponent->RegisterAbilitySystemComponent(PS->GetAbilitySystemComponent());
	}
	
#endif
	
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
