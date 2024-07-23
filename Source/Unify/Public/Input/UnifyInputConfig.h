// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "EnhancedInputComponent.h"
#include "UnifyInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * FInputMappingContextAndPriority
 */
USTRUCT(BlueprintType)
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	FInputMappingContextAndPriority();

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	/** Higher priority input mappings will be prioritized over mappings with a lower priority. */
	UPROPERTY(EditAnywhere, Category="Input")
	int32 Priority;
	
};

/**
 * FUnifyInputAction
 *
 * Struct used to map an input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FUnifyInputAction
{
	GENERATED_BODY()

	FUnifyInputAction();

	bool IsValid() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Input"), Category = "Input")
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<ETriggerEvent> PressedTriggers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<ETriggerEvent> ReleasedTriggers;
	
};

/**
 * UUnifyInputConfig
 * 
 * Non-mutable data asset that contains input configuration properties.
 */
UCLASS()
class UNIFY_API UUnifyInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UUnifyInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Unify|Input")
	FUnifyInputAction FindNativeActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Unify|Input")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Unify|Input")
	FUnifyInputAction FindAbilityActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Unify|Input")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintPure, Category = "Unify|Input")
	const TArray<FUnifyInputAction>& GetNativeInputActions() const;

	UFUNCTION(BlueprintPure, Category = "Unify|Input")
	const TArray<FUnifyInputAction>& GetAbilityInputActions() const;

	UFUNCTION(BlueprintPure, Category = "Unify|Input")
	const TArray<FInputMappingContextAndPriority>& GetInputMappingContexts() const;

protected:

	/**
	 * List of input actions used by the owner.
	 * These input actions are mapped to a gameplay tag and must be manually bound.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"), Category = "Input")
	TArray<FUnifyInputAction> NativeInputActions;

	/**
	 * List of input actions used by the owner.
	 * These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"), Category = "Input")
	TArray<FUnifyInputAction> AbilityInputActions;

	/**
	 * List of input mapping contexts used by the owner.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<FInputMappingContextAndPriority> InputMappings;
	
};
