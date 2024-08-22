// Copyright Nazake. All Rights Reserved.

#include "UnifyGameplayTags.h"
#include "GameplayTagsManager.h"
#include "UnifyLogging.h"

FUnifyGameplayTags FUnifyGameplayTags::GameplayTags;

const FUnifyGameplayTags& FUnifyGameplayTags::Get()
{
	return GameplayTags;
}

void FUnifyGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

FGameplayTag FUnifyGameplayTags::FindTagByString(const FString& TagString, const bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				UNIFY_LOG(Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}

void FUnifyGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Ability_Action_Jump, "Ability.Action.Jump", "An ability that allows the player to perform jump action.");
	
	AddTag(Ability_Activation_Failure_IsDead, "Ability.Activation.Failure.IsDead", "Ability failed to activate because its owner is dead.");
	AddTag(Ability_Activation_Failure_Cooldown, "Ability.Activation.Failure.Cooldown", "Ability failed to activate because it is on cool down.");
	AddTag(Ability_Activation_Failure_Cost, "Ability.Activation.Failure.Cost", "Ability failed to activate because it did not pass the cost checks.");
	AddTag(Ability_Activation_Failure_TagsBlocked, "Ability.Activation.Failure.TagsBlocked", "Ability failed to activate because tags are blocking it.");
	AddTag(Ability_Activation_Failure_TagsMissing, "Ability.Activation.Failure.TagsMissing", "Ability failed to activate because tags are missing.");
	AddTag(Ability_Activation_Failure_Networking, "Ability.Activation.Failure.Networking", "Ability failed to activate because it did not pass the network checks.");
	AddTag(Ability_Activation_Failure_ActivationGroup, "Ability.Activation.Failure.ActivationGroup", "Ability failed to activate because of its activation group.");

	AddTag(Ability_Input_Blocked, "Ability.Input.Blocked", "Ability input is blocked.");
	AddTag(Ability_Behavior_Persistent, "Ability.Behavior.Persistent", "An ability with this type tag should not be canceled due to death.");

	AddTag(Input_Move, "Input.Move", "Move input.");
	AddTag(Input_Look, "Input.Look", "Look (mouse) input.");
	AddTag(Input_Jump, "Input.Jump", "Jump input.");
	AddTag(Input_Crouch, "Input.Crouch", "Crouch input.");

#if COMPILE_GAMEPLAY_CONTAINERS
	
	AddTag(Input_Hotbar_1, "Input.Hotbar.1", "Toggle hotbar slot 1.");
	AddTag(Input_Hotbar_2, "Input.Hotbar.2", "Toggle hotbar slot 2.");
	AddTag(Input_Hotbar_3, "Input.Hotbar.3", "Toggle hotbar slot 3.");
	AddTag(Input_Hotbar_4, "Input.Hotbar.4", "Toggle hotbar slot 4.");
	AddTag(Input_Hotbar_5, "Input.Hotbar.5", "Toggle hotbar slot 5.");
	AddTag(Input_Hotbar_6, "Input.Hotbar.6", "Toggle hotbar slot 6.");
	
#endif

	AddTag(Input_OpenInventory, "Input.OpenInventory", "Open inventory menu input.");
	
	AddTag(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	AddTag(GameplayEvent_Respawn, "GameplayEvent.Respawn", "Event that fires once a player reset or respawn is executed.");
	AddTag(GameplayEvent_RequestRespawn, "GameplayEvent.RequestRespawn", "Event to request a player's pawn to be instantly replaced with a new one at a valid spawn location.");
	
	AddTag(Gameplay_MovementStopped, "Gameplay.MovementStopped", "Movement stopped.");
	
	AddTag(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
	AddTag(SetByCaller_Healing, "SetByCaller.Healing", "SetByCaller tag used by healing gameplay effects.");

	AddTag(Cheat_GodMode, "Cheat.GodMode", "GodMode cheat is active on the owner.");

	AddTag(Status_Crouching, "Status.Crouching", "Target is crouching.");
	AddTag(Status_Death, "Status.Death", "Target has the death status.");
	AddTag(Status_Death_KnockedOut, "Status.Death.KnockedOut", "Target has begun the death process.");
	AddTag(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");

	AddTag(UI_Layer_Game, "UI.Layer.Game", "Displays UI elements related to the in-game environment.");
	AddTag(UI_Layer_GameMenus, "UI.Layer.GameMenus", "Displays UI elements related to in-game menus.");
	AddTag(UI_Layer_Menus, "UI.Layer.Menus", "Displays UI elements related to general menus.");
	AddTag(UI_Layer_Modals, "UI.Layer.Modals", "Displays UI elements related to modal windows.");

	AddTag(UI_ExtensionPoint_Reticle, "UI.ExtensionPoint.Reticle", "Reticle UI extension point.");
	AddTag(UI_ExtensionPoint_Interaction, "UI.ExtensionPoint.Interaction", "Interaction UI extension point.");
	
	AddMovementModeTag(Movement_Mode_Walking, "Movement.Mode.Walking", MOVE_Walking);
	AddMovementModeTag(Movement_Mode_NavWalking, "Movement.Mode.NavWalking", MOVE_NavWalking);
	AddMovementModeTag(Movement_Mode_Falling, "Movement.Mode.Falling", MOVE_Falling);
	AddMovementModeTag(Movement_Mode_Swimming, "Movement.Mode.Swimming", MOVE_Swimming);
	AddMovementModeTag(Movement_Mode_Flying, "Movement.Mode.Flying", MOVE_Flying);
	AddMovementModeTag(Movement_Mode_Custom, "Movement.Mode.Custom", MOVE_Custom);
}

void FUnifyGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FUnifyGameplayTags::AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const uint8 MovementMode)
{
	AddTag(OutTag, TagName, "Character movement mode tag.");
	GameplayTags.MovementModeTagMap.Add(MovementMode, OutTag);
}

void FUnifyGameplayTags::AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const uint8 CustomMovementMode)
{
	AddTag(OutTag, TagName, "Character custom movement mode tag.");
	GameplayTags.CustomMovementModeTagMap.Add(CustomMovementMode, OutTag);
}
