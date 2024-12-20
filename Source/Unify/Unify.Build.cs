// Copyright Nazake. All Rights Reserved.

using UnrealBuildTool;

public class Unify : ModuleRules
{
	public Unify(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			// "GameplayContainers",
			// "GameplayInteraction",
		});
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"Engine", 
			"AIModule",
			"InputCore", 
			"CoreOnline",
			"CoreUObject", 
			"EnhancedInput",
			"ModularGameplay",
			"GameplayAbilities",
			"ModularGameplayActors",
			"GameplayMessageRuntime",
			"CommonLoadingScreen",
			"GameplayTasks",
			"GameplayTags",
			"CommonInput",
			"AsyncMixin",
			"UIExtension",
			"CommonGame",
			"CommonUser",
			"CommonUI",
			"NetCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UMG", 
			"Slate", 
			"SlateCore",
		});
		
		// Generate compile errors if using DrawDebug functions in test/shipping builds.
		PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");

		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
	}
}
