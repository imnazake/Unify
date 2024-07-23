// Copyright Frogster Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * FUnifyGameplayTags
 *
 *	Singleton containing native gameplay tags.
 */
struct UNIFY_API FUnifyGameplayTags
{
	static const FUnifyGameplayTags& Get();

	static void InitializeNativeTags();

	static FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	
	FGameplayTag Ability_Action_Jump;
	
	FGameplayTag Ability_Activation_Failure_IsDead;
	FGameplayTag Ability_Activation_Failure_Cooldown;
	FGameplayTag Ability_Activation_Failure_Cost;
	FGameplayTag Ability_Activation_Failure_TagsBlocked;
	FGameplayTag Ability_Activation_Failure_TagsMissing;
	FGameplayTag Ability_Activation_Failure_Networking;
	FGameplayTag Ability_Activation_Failure_ActivationGroup;
	
	FGameplayTag Ability_Input_Blocked;
	FGameplayTag Ability_Behavior_Persistent;

	FGameplayTag Input_Move;
	FGameplayTag Input_Look;
	FGameplayTag Input_Jump;
	FGameplayTag Input_Crouch;

	FGameplayTag Input_Hotbar_1;
	FGameplayTag Input_Hotbar_2;
	FGameplayTag Input_Hotbar_3;
	FGameplayTag Input_Hotbar_4;
	FGameplayTag Input_Hotbar_5;
	FGameplayTag Input_Hotbar_6;

	FGameplayTag Input_OpenInventory;

	FGameplayTag GameplayEvent_Death;
	FGameplayTag GameplayEvent_Respawn;
	FGameplayTag GameplayEvent_RequestRespawn;

	FGameplayTag Gameplay_MovementStopped;

	FGameplayTag SetByCaller_Damage;
	FGameplayTag SetByCaller_Heal;

	FGameplayTag Cheat_GodMode;

	FGameplayTag Status_Crouching;
	FGameplayTag Status_Death;
	FGameplayTag Status_Death_KnockedOut;
	FGameplayTag Status_Death_Dead;

	FGameplayTag UI_Layer_Game;
	FGameplayTag UI_Layer_GameMenus;
	FGameplayTag UI_Layer_Menus;
	FGameplayTag UI_Layer_Modals;
	
	FGameplayTag UI_ExtensionPoint_Reticle;
	FGameplayTag UI_ExtensionPoint_Interaction;

	FGameplayTag Movement_Mode_Walking;
	FGameplayTag Movement_Mode_NavWalking;
	FGameplayTag Movement_Mode_Falling;
	FGameplayTag Movement_Mode_Swimming;
	FGameplayTag Movement_Mode_Flying;
	FGameplayTag Movement_Mode_Custom;

	TMap<uint8, FGameplayTag> MovementModeTagMap;
	TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

protected:

	void AddAllTags(UGameplayTagsManager& Manager);
	static void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	static void AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode);
	static void AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode);

private:

	static FUnifyGameplayTags GameplayTags;
	
};

