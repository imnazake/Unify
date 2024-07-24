// Copyright Nazake. All Rights Reserved.

#include "Character/UnifyCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/UnifyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "UnifyGameplayTags.h"

namespace BastionCharacter
{
	static float GroundTraceDistance = 100000.0f;
	FAutoConsoleVariableRef CVar_GroundTraceDistance(TEXT("BastionCharacter.GroundTraceDistance"), GroundTraceDistance, TEXT("Distance to trace down when generating ground information."), ECVF_Cheat);
};

FUnifyCharacterGroundInfo::FUnifyCharacterGroundInfo() : LastUpdateFrame(0), GroundDistance(0.0f)
{
}

UUnifyCharacterMovementComponent::UUnifyCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUnifyCharacterMovementComponent::SimulateMovement(const float DeltaTime)
{
	Super::SimulateMovement(DeltaTime);
}

FRotator UUnifyCharacterMovementComponent::GetDeltaRotation(const float DeltaTime) const
{
	if (const UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(FUnifyGameplayTags::Get().Gameplay_MovementStopped))
		{
			return FRotator(0,0,0);
		}
	}
	
	return Super::GetDeltaRotation(DeltaTime);
}

float UUnifyCharacterMovementComponent::GetMaxSpeed() const
{
	if (const UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(FUnifyGameplayTags::Get().Gameplay_MovementStopped))
		{
			return 0.0f;
		}
	}
	
	return Super::GetMaxSpeed();
}

const FUnifyCharacterGroundInfo& UUnifyCharacterMovementComponent::GetGroundInfo()
{
	if (!CharacterOwner || (GFrameCounter == CachedGroundInfo.LastUpdateFrame))
	{
		return CachedGroundInfo;
	}

	if (MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHitResult = CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* MyCapsuleComponent = CharacterOwner->GetCapsuleComponent();
		check(MyCapsuleComponent);

		const float CapsuleHalfHeight = MyCapsuleComponent->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - BastionCharacter::GroundTraceDistance - CapsuleHalfHeight));

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BastionCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(QueryParams, ResponseParam);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

		CachedGroundInfo.GroundHitResult = HitResult;
		CachedGroundInfo.GroundDistance = BastionCharacter::GroundTraceDistance;

		if (MovementMode == MOVE_NavWalking)
		{
			CachedGroundInfo.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;

	return CachedGroundInfo;
}

