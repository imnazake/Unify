// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "UnifySpectatorPawn.generated.h"

/**
 * AUnifySpectatorPawn
 */
UCLASS()
class UNIFY_API AUnifySpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:

	AUnifySpectatorPawn();
	
	virtual void BeginPlay() override;
	
};
