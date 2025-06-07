// Copyright Nazake. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UnifyAssetManager.generated.h"

/**
 * UUnifyAssetManager
 */
UCLASS(Config = "Game")
class UNIFY_API UUnifyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	
	UUnifyAssetManager();

	/** Returns the AssetManager singleton object. */
	static UUnifyAssetManager& Get();

	/** Returns the asset referenced by a TSoftObjectPtr. This will synchronously load the asset if it's not already loaded. */
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	/** Returns the subclass referenced by a TSoftClassPtr. This will synchronously load the asset if it's not already loaded. */
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	/** Logs all assets currently loaded and tracked by the asset manager. */
	static void DumpLoadedAssets();

protected:

	//~UAssetManager Interface
	virtual void StartInitialLoading() override;
	//~UAssetManager Interface

	/** Thread safe way of adding a loaded asset to keep in memory. */
	void AddLoadedAsset(const UObject* Asset);

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

private:

	/** Assets loaded and tracked by the asset manager. */
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	/** Used for a scope lock when modifying the list of load assets. */
	FCriticalSection MyCriticalSection;
	
};

template<typename AssetType>
AssetType* UUnifyAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> UUnifyAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}