// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "UnifyGameSession.generated.h"

/**
 * AUnifyGameSession
 */
UCLASS()
class UNIFY_API AUnifyGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	AUnifyGameSession();

	virtual void BeginPlay() override;

};
