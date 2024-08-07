// Copyright Nazake. All Rights Reserved.

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
	
	void HandleBackAction() const;

protected:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true), Category = "Back")
	TSoftClassPtr<UCommonActivatableWidget> MenuWidgetClass;
	
};
