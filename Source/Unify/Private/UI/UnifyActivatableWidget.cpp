// Copyright Nazake. All Rights Reserved.

#include "UI/UnifyActivatableWidget.h"

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
