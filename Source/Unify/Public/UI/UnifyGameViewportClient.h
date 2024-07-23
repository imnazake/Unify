// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "UnifyGameViewportClient.generated.h"

/**
 * UUnifyGameViewportClient
 */
UCLASS()
class UNIFY_API UUnifyGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	
	UUnifyGameViewportClient();
	
	virtual void Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice) override;
	
};
