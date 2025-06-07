// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UnifyCharacterMovementComponent.generated.h"

/**
 * FUnifyCharacterGroundInfo
 *
 * Information about the ground under the character. It only gets updated as needed.
 */
USTRUCT(BlueprintType)
struct FUnifyCharacterGroundInfo
{
	GENERATED_BODY()

	FUnifyCharacterGroundInfo();

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/**
 * UUnifyCharacterMovementComponent
 */
UCLASS(Config = "Game")
class UNIFY_API UUnifyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UUnifyCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual void SimulateMovement(float DeltaTime) override;

	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Unify|CharacterMovement")
	const FUnifyCharacterGroundInfo& GetGroundInfo();

protected:

	/** Cached ground info for the character. Do not access this directly! It's only updated when accessed via GetGroundInfo(). */
	FUnifyCharacterGroundInfo CachedGroundInfo;
	
};
