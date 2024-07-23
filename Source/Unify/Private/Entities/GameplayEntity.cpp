// Copyright Frogster Games. All Rights Reserved.

#include "Entities/GameplayEntity.h"

AGameplayEntity::AGameplayEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplayEntity::BeginPlay()
{
	Super::BeginPlay();
}
