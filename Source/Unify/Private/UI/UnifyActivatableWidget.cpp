// Copyright Nazake. All Rights Reserved.

#include "UI/UnifyActivatableWidget.h"
#include "Input/CommonUIInputTypes.h"

UUnifyActivatableWidget::UUnifyActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), InputConfig(EUnifyWidgetInputMode::Default), GameMouseCaptureMode(EMouseCaptureMode::CapturePermanently)
{
}

TOptional<FUIInputConfig> UUnifyActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
		case EUnifyWidgetInputMode::GameAndMenu:
			return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
		case EUnifyWidgetInputMode::Game:
			return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
		case EUnifyWidgetInputMode::Menu:
			return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		case EUnifyWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}

void UUnifyActivatableWidget::RegisterBinding(FDataTableRowHandle InputAction, const FInputActionExecutedDelegate& Callback, FUnifyInputActionBindingHandle& BindingHandle)
{
	FBindUIActionArgs BindArgs(InputAction, FSimpleDelegate::CreateLambda([InputAction, Callback]()
	{
		Callback.ExecuteIfBound(InputAction.RowName);
	}));
	
	BindArgs.bDisplayInActionBar = true;
	
	BindingHandle.Handle = RegisterUIActionBinding(BindArgs);
	BindingHandles.Add(BindingHandle.Handle);
}

void UUnifyActivatableWidget::UnregisterBinding(FUnifyInputActionBindingHandle BindingHandle)
{
	if (BindingHandle.Handle.IsValid())
	{
		BindingHandle.Handle.Unregister();
		BindingHandles.Remove(BindingHandle.Handle);
	}
}

void UUnifyActivatableWidget::UnregisterAllBindings()
{
	for (FUIActionBindingHandle Handle : BindingHandles)
	{
		Handle.Unregister();
	}
	
	BindingHandles.Empty();
}
