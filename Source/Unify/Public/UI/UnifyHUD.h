// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UnifyHUD.generated.h"

class UCommonActivatableWidget;

/**
 * AUnifyHUD
 */
UCLASS()
class UNIFY_API AUnifyHUD : public AHUD
{
	GENERATED_BODY()

public:

	AUnifyHUD(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	//~AHUD Interface
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
	//~AHUD Interface

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "HUD Widget Class"), Category = "HUD")
	TSubclassOf<UCommonActivatableWidget> HUDWidgetClass;
	
};
