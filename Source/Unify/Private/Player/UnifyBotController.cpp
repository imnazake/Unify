// Copyright Nazake. All Rights Reserved.

#include "Player/UnifyBotController.h"

AUnifyBotController::AUnifyBotController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUnifyBotController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnifyBotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

