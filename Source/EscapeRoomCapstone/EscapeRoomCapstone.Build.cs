// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EscapeRoomCapstone : ModuleRules
{
	public EscapeRoomCapstone(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"EscapeRoomCapstone",
			"EscapeRoomCapstone/Variant_Horror",
			"EscapeRoomCapstone/Variant_Horror/UI",
			"EscapeRoomCapstone/Variant_Shooter",
			"EscapeRoomCapstone/Variant_Shooter/AI",
			"EscapeRoomCapstone/Variant_Shooter/UI",
			"EscapeRoomCapstone/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
