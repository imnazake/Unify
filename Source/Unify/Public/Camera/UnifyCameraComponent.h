// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "UnifyCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNIFY_API UUnifyCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:

	UUnifyCameraComponent();
	
};
