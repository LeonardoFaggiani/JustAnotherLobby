// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JustAnotherLobby : ModuleRules
{
	public JustAnotherLobby(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "OnlineSubsystem",
            "SlateCore",
            "MoviePlayer"
        });

        PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "UMG",
                    "Slate",
                    "CommonUI",
                    "JustAnotherLobbyLoadingScreen",
                    "Slate",
                    "SlateCore",
                    "InputCore",
                    "MoviePlayer",
                    "GameplayAbilities",
                    "GameplayTags",
                    "GameplayTasks"
                });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
