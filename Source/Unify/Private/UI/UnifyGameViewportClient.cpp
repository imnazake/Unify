// Copyright Nazake. All Rights Reserved.

#include "UI/UnifyGameViewportClient.h"

UUnifyGameViewportClient::UUnifyGameViewportClient()
{
}

void UUnifyGameViewportClient::Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, const bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
}
