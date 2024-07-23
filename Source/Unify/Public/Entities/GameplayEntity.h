// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEntity.generated.h"

UCLASS()
class UNIFY_API AGameplayEntity : public AActor
{
	GENERATED_BODY()

public:
	
	AGameplayEntity();
	
	virtual void BeginPlay() override;
	
};
