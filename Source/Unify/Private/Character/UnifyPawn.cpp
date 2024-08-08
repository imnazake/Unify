// Copyright Nazake. All Rights Reserved.

#include "Character/UnifyPawn.h"
#include "Input/UnifyInputComponent.h"
#include "AbilitySystem/UnifyAbilitySystemComponent.h"
#include "AbilitySystem/UnifyAbilitySet.h"
#include "Player/UnifyPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "UnifyGameplayTags.h"
#include "InputMappingContext.h"
#include "Player/UnifyPlayerController.h"

AUnifyPawn::AUnifyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUnifyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UUnifyInputComponent* UnifyInputComponent = CastChecked<UUnifyInputComponent>(PlayerInputComponent);
	InitializePlayerInput(UnifyInputComponent);
}

void AUnifyPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, /*Avatar*/ this);

		for (UUnifyAbilitySet* AbilitySet: AbilitySets)
		{
			if (AbilitySet)
			{
				FUnifyAbilitySetGrantedHandles* Handles = nullptr;
				AbilitySet->GrantToAbilitySystem(PS->GetUnifyAbilitySystemComponent(), Handles);
				AbilitySetsGrantedHandles.Add(Handles);
			}
		}
	}
}

void AUnifyPawn::UnPossessed()
{
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		for (FUnifyAbilitySetGrantedHandles* Handle: AbilitySetsGrantedHandles)
		{
			if (Handle)
			{
				Handle->RevokeFromAbilitySystem(PS->GetUnifyAbilitySystemComponent());
			}
		}
		
		PS->GetAbilitySystemComponent()->ClearAllAbilities();
		PS->GetAbilitySystemComponent()->ClearActorInfo();
	}

	Super::UnPossessed();
}

void AUnifyPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, /*Avatar*/ this);
	}
}

void AUnifyPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

TArray<UUnifyInputConfig*> AUnifyPawn::GetInputConfigs() const
{
	return InputConfigs;
}

TArray<UUnifyAbilitySet*> AUnifyPawn::GetAbilitySets() const
{
	return AbilitySets;
}

AUnifyPlayerController* AUnifyPawn::GetUnifyPlayerController() const
{
	return GetController<AUnifyPlayerController>();
}

void AUnifyPawn::Input_AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		if (UUnifyAbilitySystemComponent* ASC = PS->GetUnifyAbilitySystemComponent())
		{
			ASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void AUnifyPawn::Input_AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		if (UUnifyAbilitySystemComponent* ASC = PS->GetUnifyAbilitySystemComponent())
		{
			ASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void AUnifyPawn::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	if (Value.X != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Value.Y);
	}
}

void AUnifyPawn::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

TArray<FUnifyAbilitySetGrantedHandles*> AUnifyPawn::GetAbilitySetGrantedHandles() const
{
	return AbilitySetsGrantedHandles;
}

UAbilitySystemComponent* AUnifyPawn::GetAbilitySystemComponent() const
{
	if (const AUnifyPlayerState* MyPlayerState = Cast<AUnifyPlayerState>(GetPlayerState()))
	{
		return MyPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

UUnifyAbilitySystemComponent* AUnifyPawn::GetUnifyAbilitySystemComponent() const
{
	if (const AUnifyPlayerState* MyPlayerState = Cast<AUnifyPlayerState>(GetPlayerState()))
	{
		return MyPlayerState->GetUnifyAbilitySystemComponent();
	}

	return nullptr;
}

void AUnifyPawn::InitializePlayerInput(UUnifyInputComponent* PlayerInputComponent)
{
	const FUnifyGameplayTags& GameplayTags = FUnifyGameplayTags::Get();

	for (const UUnifyInputConfig* InputConfig: InputConfigs)
	{
		if (InputConfig)
		{
			TArray<uint32> BindHandles;
			PlayerInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*Out*/ BindHandles);

			// Bind Move & Look Input
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Move, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Look, this, &ThisClass::Input_Look, /*bLogIfNotFound=*/ false);
		}
	}
}

