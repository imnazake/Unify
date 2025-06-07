// Copyright Nazake. All Rights Reserved.

#include "Character/UnifySpectatorPawn.h"

AUnifySpectatorPawn::AUnifySpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUnifySpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
}