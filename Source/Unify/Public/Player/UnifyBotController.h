// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnifyBotController.generated.h"

UCLASS()
class UNIFY_API AUnifyBotController : public AAIController
{
	GENERATED_BODY()

public:

	AUnifyBotController();

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
};
