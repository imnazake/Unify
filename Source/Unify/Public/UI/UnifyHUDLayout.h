// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnifyActivatableWidget.h"
#include "UnifyHUDLayout.generated.h"

/**
 * UUnifyHUDLayout
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class UNIFY_API UUnifyHUDLayout : public UUnifyActivatableWidget
{
	GENERATED_BODY()

public:
	
	UUnifyHUDLayout(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

protected:
	
	void HandleEscapeAction() const;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> MainMenuClass;
};
