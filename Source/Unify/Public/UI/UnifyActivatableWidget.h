// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UnifyActivatableWidget.generated.h"

struct FUIInputConfig;

/**
 * EUnifyWidgetInputMode
 */
UENUM(BlueprintType)
enum class EUnifyWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

/**
 * UUnifyActivatableWidget
 */
UCLASS()
class UNIFY_API UUnifyActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UUnifyActivatableWidget(const FObjectInitializer& ObjectInitializer);

	//~UCommonActivatableWidget Interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~UCommonActivatableWidget Interface

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	EUnifyWidgetInputMode InputConfig;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	EMouseCaptureMode GameMouseCaptureMode;
	
};
