// Copyright Nazake. All Rights Reserved.

#include "AbilitySystem/Abilities/UnifyGameplayAbility_Interaction.h"
#include "Core/Tasks/GameplayInteractionTask_WaitInteraction.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Core/GameplayEntityInteractionComponent.h"
#include "Core/IGameplayInteractionInterface.h"
#include "Core/GameplayInteractionComponent.h"
#include "Core/GameplayInteractionLibrary.h"
#include "Core/GameplayInteractionOption.h"
#include "Core/IGameplayInteractiveEntityInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayInteraction.h"
#include "GameplayInteractionTags.h"
#include "Camera/CameraComponent.h"

UUnifyGameplayAbility_Interaction::UUnifyGameplayAbility_Interaction()
{
	bServerRespectsRemoteAbilityCancellation = true;
	bReplicateInputDirectly = true;

	CurrentComponent = nullptr;
	CurrentOption = nullptr;
	CurrentEntity = nullptr;
	bWasExecuted = false;
	CurrentTargetDirection = FVector::ZeroVector;

	ActivationBlockedTags.AddTag(GameplayInteractionTags::TAG_Gameplay_Interaction_Unauthorized);
	ActivationBlockedTags.AddTag(GameplayInteractionTags::TAG_Gameplay_Interaction_Blocked);
	ActivationBlockedTags.AddTag(GameplayInteractionTags::TAG_Gameplay_Interaction_Ongoing);
	ActivationBlockedTags.AddTag(GameplayInteractionTags::TAG_Gameplay_Interaction_Busy);

	ActivationOwnedTags.AddTag(GameplayInteractionTags::TAG_Gameplay_Interaction_Ongoing);
}

void UUnifyGameplayAbility_Interaction::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (CurrentOption && CurrentOption->InteractionType == EGameplayInteractionType::Timed && !bWasExecuted)
	{
		CancelInteraction();
	}

	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UUnifyGameplayAbility_Interaction::CanExecuteInteraction() const
{
	if (IsLocallyControlled())
	{
		if (!CurrentEntity || !CurrentComponent || !CurrentOption)
		{
			return false;
		}

		FGameplayTagContainer FailureTags;
		return UGameplayInteractionLibrary::CanActivateInteractionOption(
			GetAbilitySystemComponentFromActorInfo(),
			CurrentEntity,
			CurrentComponent,
			CurrentOption,
			FailureTags
		);
	}

	if (!HasAuthority(&CurrentActivationInfo) || !HasValidInteractionData())
	{
		return false;
	}

	UGameplayInteractionComponent* InteractionComponent = GetPlayerInteractionComponent();
	AActor* Avatar = GetAvatarActorFromActorInfo();
	UCameraComponent* Camera = UGameplayInteractionLibrary::GetInteractionCameraComponent(Avatar);

	if (!InteractionComponent || !Avatar || !Camera)
	{
		return false;
	}

	const float MaxRange = InteractionComponent->GetScanRange() + InteractionComponent->GetMaxDistanceTolerance();
	const FVector CamLocation = Camera->GetComponentLocation();
	const FRotator CamRotation = Camera->GetComponentRotation();

	if (bValidateRemoteClientCameraOrigin &&
		FVector::DistSquared(CamLocation, Avatar->GetActorLocation()) > FMath::Square(InteractionComponent->GetMaxDistanceTolerance()))
	{
		return false;
	}

	if (bValidateRemoteClientDirection)
	{
		const FVector ServerForward = CamRotation.Vector();
		if (FVector::DotProduct(ServerForward, CurrentTargetDirection) < 0.98f)
		{
			return false;
		}
	}

	const FVector TraceStart = CamLocation + InteractionComponent->GetTraceOffset();
	const FVector TraceEnd = TraceStart + (CurrentTargetDirection * MaxRange);

	FLineTraceResult TraceResult;
	UGameplayInteractionLibrary::PerformLineTrace(GetWorld(), TraceStart, TraceEnd, ECC_Interaction, { Avatar }, TraceResult);

	if (!TraceResult.IsValid() || TraceResult.HitActor != CurrentEntity)
	{
		return false;
	}

	if (bValidateRemoteClientDistance)
	{
		const float DistanceSq = FVector::DistSquared(Avatar->GetActorLocation(), TraceResult.HitActor->GetActorLocation());
		if (DistanceSq > FMath::Square(MaxRange))
		{
			return false;
		}
	}

	if (bValidateRemoteClientAim)
	{
		const FVector ToHit = (TraceResult.HitLocation - TraceStart).GetSafeNormal();
		if (FVector::DotProduct(CurrentTargetDirection, ToHit) < InteractionComponent->GetMinAimDot())
		{
			return false;
		}
	}

	FGameplayTagContainer FailureTags;
	return UGameplayInteractionLibrary::CanActivateInteractionOption(
		GetAbilitySystemComponentFromActorInfo(),
		TraceResult.HitActor,
		TraceResult.HitComponent,
		CurrentOption,
		FailureTags
	);
}

bool UUnifyGameplayAbility_Interaction::HasValidInteractionData() const
{
	return CurrentEntity && CurrentOption && !CurrentTargetDirection.IsZero();
}

APlayerController* UUnifyGameplayAbility_Interaction::GetPlayerControllerFromActorInfo() const
{
	return GetActorInfo().PlayerController.Get();
}

UGameplayInteractionComponent* UUnifyGameplayAbility_Interaction::GetPlayerInteractionComponent() const
{
	if (IGameplayInteractionInterface* InteractionInterface = Cast<IGameplayInteractionInterface>(GetControllerFromActorInfo()))
	{
		return InteractionInterface->GetInteractionComponent();
	}

	return nullptr;
}

AActor* UUnifyGameplayAbility_Interaction::GetEntityInView() const
{
	if (UGameplayInteractionComponent* PlayerInteractionComponent = GetPlayerInteractionComponent())
	{
		return PlayerInteractionComponent->GetEntityInView();
	}

	return nullptr;
}

AActor* UUnifyGameplayAbility_Interaction::GetCachedEntityFromInteraction() const
{
	if (const UGameplayInteractionComponent* PlayerInteractionComponent = GetPlayerInteractionComponent())
	{
		return PlayerInteractionComponent->GetCachedEntityFromInteraction();
	}

	return nullptr;
}

UActorComponent* UUnifyGameplayAbility_Interaction::GetEntityComponentInView() const
{
	if (UGameplayInteractionComponent* PlayerInteractionComponent = GetPlayerInteractionComponent())
	{
		return PlayerInteractionComponent->GetEntityComponentInView();
	}

	return nullptr;
}

UActorComponent* UUnifyGameplayAbility_Interaction::GetCachedEntityComponentInView() const
{
	if (UGameplayInteractionComponent* PlayerInteractionComponent = GetPlayerInteractionComponent())
	{
		return PlayerInteractionComponent->GetCachedEntityComponentFromInteraction();
	}

	return nullptr;
}

UGameplayInteractionOption* UUnifyGameplayAbility_Interaction::GetCurrentOption() const
{
	return CurrentOption;
}

AActor* UUnifyGameplayAbility_Interaction::GetCurrentEntity() const
{
	return CurrentEntity;
}

UActorComponent* UUnifyGameplayAbility_Interaction::GetCurrentComponent() const
{
	return CurrentComponent;
}

bool UUnifyGameplayAbility_Interaction::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	UGameplayInteractionComponent* InteractionComponent = GetPlayerInteractionComponent();
	if (!InteractionComponent)
	{
		return false;
	}

	if (IsLocallyControlled())
	{
		AActor* EntityInView = InteractionComponent->GetEntityInView();
		UActorComponent* ComponentInView = InteractionComponent->GetEntityComponentInView();
		UGameplayInteractionOption* SelectedOption = InteractionComponent->GetSelectedInteractionOption();

		bResult &= (EntityInView && ComponentInView && SelectedOption);

		FGameplayTagContainer FailureTags;
		bResult &= UGameplayInteractionLibrary::CanActivateInteractionOption(
			GetAbilitySystemComponentFromActorInfo(),
			EntityInView,
			ComponentInView,
			SelectedOption,
			FailureTags
		);
	}

	return bResult;
}

void UUnifyGameplayAbility_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UGameplayInteractionComponent* PlayerInteractionComponent = GetPlayerInteractionComponent();
	PlayerInteractionComponent->CacheEntityInViewForInteraction();

	CurrentEntity = PlayerInteractionComponent->GetCachedEntityFromInteraction();
	CurrentComponent = PlayerInteractionComponent->GetCachedEntityComponentFromInteraction();
	CurrentOption = PlayerInteractionComponent->GetSelectedInteractionOption();
	CurrentTargetDirection = PlayerInteractionComponent->GetCachedInteractionDirection();

	UGameplayInteractionTask_WaitInteraction* WaitTask = UGameplayInteractionTask_WaitInteraction::WaitInteraction(this, CurrentEntity, CurrentOption, CurrentTargetDirection);
	WaitTask->OnTargetDataReceived.AddDynamic(this, &ThisClass::OnInteractionDataReceived);
	WaitTask->OnTargetDataCancelled.AddDynamic(this, &ThisClass::OnInteractionDataCancelled);
	WaitTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameplayInteractionTags::TAG_GameplayEvent_Interaction_Terminate, nullptr, true);
	EventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);
	EventTask->ReadyForActivation();

	if (IsLocallyControlled())
	{
		if (HasAuthority(&ActivationInfo))
		{
			if (CanExecuteInteraction())
			{
				StartInteraction();
			}
			else
			{
				CancelInteraction();
			}
		}
		else
		{
			const FGameplayAbilityTargetDataHandle DataHandle = WaitTask->GenerateTargetDataHandle();
			GetAbilitySystemComponentFromActorInfo()->ServerSetReplicatedTargetData(
				GetCurrentAbilitySpecHandle(),
				GetCurrentActivationInfo().GetActivationPredictionKey(),
				DataHandle,
				FGameplayTag::EmptyTag,
				GetAbilitySystemComponentFromActorInfo()->ScopedPredictionKey
			);

			StartInteraction();
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UUnifyGameplayAbility_Interaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	UGameplayInteractionComponent* InteractionComponent = GetPlayerInteractionComponent();

	if (ImmobilityEffectHandle.IsValid())
	{
		AbilitySystem->RemoveActiveGameplayEffect(ImmobilityEffectHandle);
		ImmobilityEffectHandle.Invalidate();
	}

	if (InteractionComponent && CurrentOption && CurrentOption->InteractionType == EGameplayInteractionType::Timed)
	{
		InteractionComponent->NotifyTimedInteractionFinished(bWasCancelled);
	}

	if (HasAuthority(&GetCurrentActivationInfoRef()))
	{
		if (IGameplayInteractiveEntityInterface* EntityInterface = Cast<IGameplayInteractiveEntityInterface>(CurrentEntity))
		{
			if (CurrentOption && CurrentOption->InteractionType == EGameplayInteractionType::Instant || (CurrentOption->InteractionType == EGameplayInteractionType::Timed && bWasExecuted))
			{
				EntityInterface->FinishInteraction(GetCurrentActorInfo()->AbilitySystemComponent.Get(), CurrentOption, bWasCancelled);

				AbilitySystem->AddLooseGameplayTags(CurrentOption->PlayerTemporaryRevokedTagsOnStart);
				RevokeInteractionEffectsAndTags(ActivationGrantResult);

				AbilitySystem->RemoveLooseGameplayTags(CurrentOption->PlayerPersistentRevokedTagsOnFinish);
				FinishGrantResult = GrantInteractionEffectsAndTags(CurrentOption->PlayerPersistentGrantedTagsOnFinish, CurrentOption->PlayerPersistentEffectsOnFinish);

				EntityInterface->GetEntityInteractionComponent()->RevokeDynamicTags(CurrentOption->TargetTemporaryGrantedTagsOnStart);
				EntityInterface->GetEntityInteractionComponent()->GrantDynamicTags(CurrentOption->TargetTemporaryRevokedTagsOnStart);

				EntityInterface->GetEntityInteractionComponent()->GrantDynamicTags(CurrentOption->TargetPersistentGrantedTagsOnFinish);
				EntityInterface->GetEntityInteractionComponent()->RevokeDynamicTags(CurrentOption->TargetPersistentRevokedTagsOnFinish);
			}
		}
	}

	ClearTimer();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnifyGameplayAbility_Interaction::OnInteractionDataReceived(AActor* Entity, UGameplayInteractionOption* Option, FVector_NetQuantizeNormal TargetDirection)
{
	if (HasAuthority(&GetCurrentActivationInfoRef()) && !IsLocallyControlled())
	{
		CurrentEntity = Entity;
		CurrentOption = Option;
		CurrentTargetDirection = TargetDirection;

		if (CanExecuteInteraction())
		{
			StartInteraction();
		}
		else
		{
			CancelInteraction();
		}
	}
}

void UUnifyGameplayAbility_Interaction::OnInteractionDataCancelled()
{
	CancelInteraction();
}

void UUnifyGameplayAbility_Interaction::OnGameplayEventReceived(FGameplayEventData Payload)
{
	if (Payload.EventTag.MatchesTagExact(GameplayInteractionTags::TAG_GameplayEvent_Interaction_Terminate))
	{
		FinishInteraction();
	}
}

void UUnifyGameplayAbility_Interaction::StartInteraction()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	UGameplayInteractionComponent* InteractionComponent = GetPlayerInteractionComponent();

	OnStartInteraction();
	BP_OnStartInteraction();

	if (CurrentOption && CurrentOption->InteractionType == EGameplayInteractionType::Instant)
	{
		if (IGameplayInteractiveEntityInterface* EntityInterface = Cast<IGameplayInteractiveEntityInterface>(CurrentEntity))
		{
			if (HasAuthority(&GetCurrentActivationInfoRef()))
			{
				EntityInterface->StartInteraction(GetCurrentActorInfo()->AbilitySystemComponent.Get(), CurrentOption);
				
				// Add player temp tags and effects on start
				ActivationGrantResult = GrantInteractionEffectsAndTags(CurrentOption->PlayerTemporaryGrantedTagsOnStart, CurrentOption->PlayerTemporaryEffectsOnStart);
			
				// Remove player temp tags on start
				AbilitySystem->RemoveLooseGameplayTags(CurrentOption->PlayerTemporaryRevokedTagsOnStart);
			
				// Add target temp tags on start
				EntityInterface->GetEntityInteractionComponent()->GrantDynamicTags(CurrentOption->TargetTemporaryGrantedTagsOnStart);
			
				// Remove target temp tags on start
				EntityInterface->GetEntityInteractionComponent()->RevokeDynamicTags(CurrentOption->TargetTemporaryRevokedTagsOnStart);
			}

			OnExecuteInteraction();
			BP_OnExecuteInteraction();

			if (HasAuthority(&GetCurrentActivationInfoRef()) && CurrentOption->bEndAbilityOnExecute)
			{
				FinishInteraction();
			}
		}
	}
	else if (CurrentOption && CurrentOption->InteractionType == EGameplayInteractionType::Timed)
	{
		if (AbilitySystem && ImmobilityEffect)
		{
			ImmobilityEffectHandle = AbilitySystem->ApplyGameplayEffectToSelf(ImmobilityEffect.GetDefaultObject(), ImmobilityEffectLevel.Value, AbilitySystem->MakeEffectContext(), GetCurrentActivationInfo().GetActivationPredictionKey());
		}

		if (InteractionComponent)
		{
			InteractionComponent->NotifyTimedInteractionStarted(CurrentOption->InteractionDuration);
		}

		StartTimer();
	}
}

void UUnifyGameplayAbility_Interaction::CancelInteraction()
{
	OnCancelInteraction();
	BP_OnCancelInteraction();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UUnifyGameplayAbility_Interaction::FinishInteraction()
{
	OnFinishInteraction();
	BP_OnFinishInteraction();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UUnifyGameplayAbility_Interaction::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::HandleTimedInteractionExecution, CurrentOption->InteractionDuration, false);
}

void UUnifyGameplayAbility_Interaction::ClearTimer()
{
	bWasExecuted = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UUnifyGameplayAbility_Interaction::HandleTimedInteractionExecution()
{
	if (!CanExecuteInteraction())
	{
		CancelInteraction();
		return;
	}

	bWasExecuted = true;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	const UGameplayInteractionComponent* InteractionComponent = GetPlayerInteractionComponent();

	InteractionComponent->NotifyTimedInteractionFinished(false);

	if (HasAuthority(&GetCurrentActivationInfoRef()))
	{
		if (IGameplayInteractiveEntityInterface* EntityInterface = Cast<IGameplayInteractiveEntityInterface>(CurrentEntity))
		{
			EntityInterface->StartInteraction(GetCurrentActorInfo()->AbilitySystemComponent.Get(), CurrentOption);
			
			// Add player temp tags and effects on start
			ActivationGrantResult = GrantInteractionEffectsAndTags(CurrentOption->PlayerTemporaryGrantedTagsOnStart, CurrentOption->PlayerTemporaryEffectsOnStart);
			
			// Remove player temp tags on start
			AbilitySystem->RemoveLooseGameplayTags(CurrentOption->PlayerTemporaryRevokedTagsOnStart);
			
			// Add target temp tags on start
			EntityInterface->GetEntityInteractionComponent()->GrantDynamicTags(CurrentOption->TargetTemporaryGrantedTagsOnStart);
			
			// Remove target temp tags on start
			EntityInterface->GetEntityInteractionComponent()->RevokeDynamicTags(CurrentOption->TargetTemporaryRevokedTagsOnStart);
		}
	}

	OnExecuteInteraction();
	BP_OnExecuteInteraction();

	if (HasAuthority(&GetCurrentActivationInfoRef()) && CurrentOption && CurrentOption->bEndAbilityOnExecute)
	{
		FinishInteraction();
	}
}

void UUnifyGameplayAbility_Interaction::OnStartInteraction()
{
}

void UUnifyGameplayAbility_Interaction::OnExecuteInteraction()
{
}

void UUnifyGameplayAbility_Interaction::OnCancelInteraction()
{
}

void UUnifyGameplayAbility_Interaction::OnFinishInteraction()
{
}

FUnifyGameplayInteractionGrantResult UUnifyGameplayAbility_Interaction::GrantInteractionEffectsAndTags(const FGameplayTagContainer& InTags, const TArray<TSubclassOf<UGameplayEffect>>& InEffects) const
{
	FUnifyGameplayInteractionGrantResult Result;
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();

	for (const TSubclassOf<UGameplayEffect>& EffectClass : InEffects)
	{
		if (!EffectClass)
		{
			continue;
		}

		const FActiveGameplayEffectHandle Handle = AbilitySystem->ApplyGameplayEffectToSelf(
			EffectClass->GetDefaultObject<UGameplayEffect>(),
			1.f,
			AbilitySystem->MakeEffectContext()
		);

		if (Handle.IsValid())
		{
			Result.EffectHandles.Add(Handle);
		}
	}

	if (!InTags.IsEmpty())
	{
		AbilitySystem->AddLooseGameplayTags(InTags);
		Result.GrantedTags = InTags;
	}

	return Result;
}

void UUnifyGameplayAbility_Interaction::RevokeInteractionEffectsAndTags(FUnifyGameplayInteractionGrantResult& InResult) const
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystem)
	{
		return;
	}

	for (const FActiveGameplayEffectHandle& Handle : InResult.EffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->RemoveActiveGameplayEffect(Handle);
		}
	}

	if (!InResult.GrantedTags.IsEmpty())
	{
		AbilitySystem->RemoveLooseGameplayTags(InResult.GrantedTags);
	}

	InResult.Reset();
}
