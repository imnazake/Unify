// Copyright Frogster Games. All Rights Reserved.

#include "UI/UnifyHUDLayout.h"
#include "UnifyGameplayTags.h"
#include "CommonUIExtensions.h"
#include "NativeGameplayTags.h"
#include "Input/CommonUIInputTypes.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_Layer_Menus, "UI.Layer.Menus");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_Action_Escape, "UI.Action.Escape");

UUnifyHUDLayout::UUnifyHUDLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUnifyHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_Action_Escape), false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
}

void UUnifyHUDLayout::HandleEscapeAction() const
{
	if (ensure(!MainMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_Layer_Menus, MainMenuClass.LoadSynchronous());
	}
}
