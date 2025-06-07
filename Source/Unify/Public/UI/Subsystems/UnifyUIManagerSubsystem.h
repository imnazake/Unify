// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "UnifyUIManagerSubsystem.generated.h"

/**
 * UUnifyUIManagerSubsystem
 */
UCLASS()
class UNIFY_API UUnifyUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:

	UUnifyUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:

	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

private:

	FTSTicker::FDelegateHandle TickHandle;
	
};
