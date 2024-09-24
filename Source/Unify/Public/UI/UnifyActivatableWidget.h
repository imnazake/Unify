// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UnifyActivatableWidget.generated.h"

struct FUIInputConfig;
struct FUIActionBindingHandle;

/**
 * FInputActionExecutedDelegate
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionExecutedDelegate, FName, ActionName);

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
 * FUnifyInputActionBindingHandle
 */
USTRUCT(BlueprintType)
struct FUnifyInputActionBindingHandle
{
	GENERATED_BODY()

	//
	FUIActionBindingHandle Handle;
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

	UFUNCTION(BlueprintCallable, Category = "Input")
	void RegisterBinding(FDataTableRowHandle InputAction, const FInputActionExecutedDelegate& Callback, FUnifyInputActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void UnregisterBinding(FUnifyInputActionBindingHandle BindingHandle);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void UnregisterAllBindings();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	EUnifyWidgetInputMode InputConfig;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	EMouseCaptureMode GameMouseCaptureMode;

private:

	TArray<FUIActionBindingHandle> BindingHandles;
	
};
