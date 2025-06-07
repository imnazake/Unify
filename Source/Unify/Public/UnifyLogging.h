// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
#include "Logging/StructuredLog.h"
#endif

#define THIS_FUNCTION FString(__FUNCTION__)
#define THIS_OBJECT_NAME GetNameSafe(this)

UNIFY_API DECLARE_LOG_CATEGORY_EXTERN(LogUnify, Display, All);

#define UNIFY_LOG(Verbosity, Format, ...) \
{ \
	UE_LOG(LogUnify, Verbosity, Format, ##__VA_ARGS__); \
}

#define UNIFY_FORMAT_LOG(Verbosity, Format, ...) \
{ \
	UE_LOGFMT(LogUnify, Verbosity, Format, ##__VA_ARGS__); \
}