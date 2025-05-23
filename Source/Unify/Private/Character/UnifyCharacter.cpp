// Copyright Nazake. All Rights Reserved.

#include "Character/UnifyCharacter.h"
#include "Character/UnifyCharacterMovementComponent.h"
#include "AbilitySystem/UnifyAbilitySystemComponent.h"
#include "AbilitySystem/UnifyAbilitySet.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/UnifyInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/UnifyPlayerState.h"
#include "Player/UnifyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "UnifyGameplayTags.h"
#include "InputMappingContext.h"

#if COMPILE_GAMEPLAY_INTERACTION
#include "Core/GameplayInteractionComponent.h"
#endif

#if COMPILE_GAMEPLAY_CONTAINERS
#include "Core/Inventory/InventoryComponent.h"
#include "Core/Equipment/EquipmentComponent.h"
#include "Core/Hotbar/HotbarComponent.h"
#endif

AUnifyCharacter::AUnifyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UUnifyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetNetCullDistanceSquared(900000000.0f);
	
	UUnifyCharacterMovementComponent* MovementComponent = CastChecked<UUnifyCharacterMovementComponent>(GetCharacterMovement());
	MovementComponent->GravityScale = 1.0f;
	MovementComponent->MaxAcceleration = 2400.0f;
	MovementComponent->BrakingFrictionFactor = 1.0f;
	MovementComponent->BrakingFriction = 6.0f;
	MovementComponent->GroundFriction = 8.0f;
	MovementComponent->BrakingDecelerationWalking = 1400.0f;
	MovementComponent->bUseControllerDesiredRotation = false;
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
	MovementComponent->GetNavAgentPropertiesRef().bCanWalk = true;
	MovementComponent->GetNavAgentPropertiesRef().bCanSwim = true;
	MovementComponent->GetNavAgentPropertiesRef().bCanJump = true;
	MovementComponent->bCanWalkOffLedgesWhenCrouching = true;
	MovementComponent->SetCrouchedHalfHeight(65.0f);

	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraBoom"));
	ThirdPersonCameraBoom->SetupAttachment(RootComponent);
	ThirdPersonCameraBoom->bUsePawnControlRotation = true;
	
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonCameraBoom);
	ThirdPersonCamera->bUsePawnControlRotation = false;
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetOwnerNoSee(false);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->bCastHiddenShadow = false;
	FirstPersonMesh->CastShadow = false;
	
	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->bCastHiddenShadow = true;
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetOnlyOwnerSee(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	BaseEyeHeight = 90.0f;
	CrouchedEyeHeight = 35.0f;

#if COMPILE_GAMEPLAY_CONTAINERS
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	EquipmentComponent->SetIsReplicated(true);
#endif
	
}

void AUnifyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AUnifyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, /*Avatar*/ this);

#if COMPILE_GAMEPLAY_CONTAINERS
		if (IGameplayContainerInterface* GameplayContainerInterface = Cast<IGameplayContainerInterface>(PS->GetPlayerController()))
		{
			GameplayContainerInterface->GetInventoryComponent()->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
			GameplayContainerInterface->GetHotbarComponent()->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
		}
		
		EquipmentComponent->RegisterWithAbilitySystem(GetAbilitySystemComponent());
#endif
		
		for (UUnifyAbilitySet* AbilitySet: AbilitySets)
		{
			if (AbilitySet)
			{
				FUnifyAbilitySetGrantedHandles* Handles = nullptr;
				AbilitySet->GrantToAbilitySystem(GetUnifyAbilitySystemComponent(), Handles);
				AbilitySetsGrantedHandles.Add(Handles);
			}
		}
	}
}

void AUnifyCharacter::UnPossessed()
{
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{

#if COMPILE_GAMEPLAY_CONTAINERS
		if (IGameplayContainerInterface* GameplayContainerInterface = Cast<IGameplayContainerInterface>(PS->GetPlayerController()))
		{
			GameplayContainerInterface->GetInventoryComponent()->UnregisterAbilitySystem();
			GameplayContainerInterface->GetHotbarComponent()->UnregisterAbilitySystem();
		}

		EquipmentComponent->UnregisterAbilitySystem();
#endif
		
		for (FUnifyAbilitySetGrantedHandles* Handle: AbilitySetsGrantedHandles)
		{
			if (Handle)
			{
				Handle->RevokeFromAbilitySystem(GetUnifyAbilitySystemComponent());
			}
		}
		
		PS->GetAbilitySystemComponent()->ClearAllAbilities();
		PS->GetAbilitySystemComponent()->ClearActorInfo();
	}
	
	Super::UnPossessed();
}

#if COMPILE_GAMEPLAY_INTERACTION

UGameplayInteractionComponent* AUnifyCharacter::GetInteractionComponent()
{
	return InteractionComponent;
}

#endif

void AUnifyCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

void AUnifyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, /*Avatar*/ this);

#if COMPILE_GAMEPLAY_CONTAINERS
		if (IGameplayContainerInterface* GameplayContainerInterface = Cast<IGameplayContainerInterface>(PS->GetPlayerController()))
		{
			GameplayContainerInterface->GetInventoryComponent()->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
			GameplayContainerInterface->GetHotbarComponent()->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
		}
		
		EquipmentComponent->RegisterWithAbilitySystem(PS->GetAbilitySystemComponent());
#endif
		
	}
}

void AUnifyCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
}

TArray<UUnifyInputConfig*> AUnifyCharacter::GetInputConfigs() const
{
	return InputConfigs;
}

TArray<UUnifyAbilitySet*> AUnifyCharacter::GetAbilitySets() const
{
	return AbilitySets;
}

AUnifyPlayerController* AUnifyCharacter::GetUnifyPlayerController() const
{
	return GetController<AUnifyPlayerController>();
}

#if COMPILE_GAMEPLAY_CONTAINERS

TArray<UGameplayContainerComponent*> AUnifyCharacter::GetGameplayContainers()
{
	TArray<UGameplayContainerComponent*> Containers;

	Containers.AddUnique(EquipmentComponent);

	return Containers;
}

UInventoryComponent* AUnifyCharacter::GetInventoryComponent()
{
	return GetUnifyPlayerController()->GetInventoryComponent();
}

UHotbarComponent* AUnifyCharacter::GetHotbarComponent()
{
	return GetUnifyPlayerController()->GetHotbarComponent();
}

UEquipmentComponent* AUnifyCharacter::GetEquipmentComponent()
{
	return EquipmentComponent;
}

#endif

void AUnifyCharacter::Input_AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		if (UUnifyAbilitySystemComponent* ASC = PS->GetUnifyAbilitySystemComponent())
		{
			ASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void AUnifyCharacter::Input_AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (const AUnifyPlayerState* PS = GetPlayerState<AUnifyPlayerState>())
	{
		if (UUnifyAbilitySystemComponent* ASC = PS->GetUnifyAbilitySystemComponent())
		{
			ASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void AUnifyCharacter::Input_Move(const FInputActionValue& InputActionValue)
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

void AUnifyCharacter::Input_Look(const FInputActionValue& InputActionValue)
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

#if COMPILE_GAMEPLAY_CONTAINERS

void AUnifyCharacter::Input_Hotbar_1()
{
	FGameplayTagContainer FailureTags;
	GetHotbarComponent()->ToggleSlot(0, FailureTags);
}

void AUnifyCharacter::Input_Hotbar_2()
{
	FGameplayTagContainer FailureTags;
	GetHotbarComponent()->ToggleSlot(1, FailureTags);
}

void AUnifyCharacter::Input_Hotbar_3()
{
	FGameplayTagContainer FailureTags;
	GetHotbarComponent()->ToggleSlot(2, FailureTags);
}

void AUnifyCharacter::Input_Hotbar_4()
{
	FGameplayTagContainer FailureTags;
	GetHotbarComponent()->ToggleSlot(3, FailureTags);
}

void AUnifyCharacter::Input_Hotbar_5()
{
	FGameplayTagContainer FailureTags;
	GetHotbarComponent()->ToggleSlot(4, FailureTags);
}

void AUnifyCharacter::Input_Hotbar_6()
{
	FGameplayTagContainer FailureTags;
	GetHotbarComponent()->ToggleSlot(5, FailureTags);
}

#endif

void AUnifyCharacter::EnableMovementAndCollision() const
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(false);
	}

	UCapsuleComponent* MyCapsule = GetCapsuleComponent();
	check(MyCapsule);
	
	MyCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MyCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	MyCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MyCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	UUnifyCharacterMovementComponent* MyMovementComponent = CastChecked<UUnifyCharacterMovementComponent>(GetCharacterMovement());
	MyMovementComponent->SetDefaultMovementMode();
}

void AUnifyCharacter::DisableMovementAndCollision() const
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* MyCapsule = GetCapsuleComponent();
	check(MyCapsule);
	
	MyCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MyCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);

	UUnifyCharacterMovementComponent* MyMovementComponent = CastChecked<UUnifyCharacterMovementComponent>(GetCharacterMovement());
	MyMovementComponent->StopMovementImmediately();
	MyMovementComponent->DisableMovement();
}

TArray<FUnifyAbilitySetGrantedHandles*> AUnifyCharacter::GetAbilitySetGrantedHandles() const
{
	return AbilitySetsGrantedHandles;
}

UAbilitySystemComponent* AUnifyCharacter::GetAbilitySystemComponent() const
{
	if (const AUnifyPlayerState* MyPlayerState = Cast<AUnifyPlayerState>(GetPlayerState()))
	{
		return MyPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

UUnifyAbilitySystemComponent* AUnifyCharacter::GetUnifyAbilitySystemComponent() const
{
	if (const AUnifyPlayerState* MyPlayerState = Cast<AUnifyPlayerState>(GetPlayerState()))
	{
		return MyPlayerState->GetUnifyAbilitySystemComponent();
	}

	return nullptr;
}

UCameraComponent* AUnifyCharacter::GetFirstPersonCamera() const
{
	return FirstPersonCamera;
}

void AUnifyCharacter::InitializePlayerInput(UUnifyInputComponent* PlayerInputComponent)
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

#if COMPILE_GAMEPLAY_CONTAINERS
			
			// Bind Hotbar Input
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Hotbar_1, this, &ThisClass::Input_Hotbar_1, /*bLogIfNotFound=*/ false);
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Hotbar_2, this, &ThisClass::Input_Hotbar_2, /*bLogIfNotFound=*/ false);
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Hotbar_3, this, &ThisClass::Input_Hotbar_3, /*bLogIfNotFound=*/ false);
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Hotbar_4, this, &ThisClass::Input_Hotbar_4, /*bLogIfNotFound=*/ false);
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Hotbar_5, this, &ThisClass::Input_Hotbar_5, /*bLogIfNotFound=*/ false);
			PlayerInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Hotbar_6, this, &ThisClass::Input_Hotbar_6, /*bLogIfNotFound=*/ false);

#endif
			
		}
	}
}

void AUnifyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AUnifyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UUnifyInputComponent* UnifyInputComponent = CastChecked<UUnifyInputComponent>(PlayerInputComponent);
	InitializePlayerInput(UnifyInputComponent);
}

