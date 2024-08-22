// Copyright Nazake. All Rights Reserved.

#include "System/UnifyAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "UnifyGameplayTags.h"
#include "UnifyLogging.h"

static FAutoConsoleCommand CVarDumpLoadedAssets(
	TEXT("Unify.DumpLoadedAssets"),
	TEXT("Shows all assets that were loaded via the asset manager and are currently in memory."),
	FConsoleCommandDelegate::CreateStatic(UUnifyAssetManager::DumpLoadedAssets)
);

UUnifyAssetManager::UUnifyAssetManager()
{
}

UUnifyAssetManager& UUnifyAssetManager::Get()
{
	check(GEngine);

	if (UUnifyAssetManager* Singleton = Cast<UUnifyAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UNIFY_LOG(Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini. It must be set to UnifyAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UUnifyAssetManager>();
}

void UUnifyAssetManager::DumpLoadedAssets()
{
	UNIFY_LOG(Log, TEXT("========== Start Dumping Loaded Assets =========="));

	for (const UObject* LoadedAsset : Get().LoadedAssets)
	{
		UNIFY_LOG(Log, TEXT("> %s"), *GetNameSafe(LoadedAsset));
	}

	UNIFY_LOG(Log, TEXT("There are %d assets in loaded pool"), Get().LoadedAssets.Num());
	UNIFY_LOG(Log, TEXT("========== Finish Dumping Loaded Assets =========="));
}

void UUnifyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FUnifyGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UUnifyAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&MyCriticalSection);
		LoadedAssets.Add(Asset);
	}
}

UObject* UUnifyAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		/*TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}*/

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 3
		if (IsInitialized())
		{
			return GetStreamableManager().LoadSynchronous(AssetPath, false);
		}
#endif
		
		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}
