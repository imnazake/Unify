// Copyright Frogster Games. All Rights Reserved.

#include "System/UnifyGameSession.h"

AUnifyGameSession::AUnifyGameSession()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUnifyGameSession::BeginPlay()
{
	Super::BeginPlay();
	
}