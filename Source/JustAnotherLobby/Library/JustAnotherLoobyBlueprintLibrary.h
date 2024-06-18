// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../JustAnotherLobbyGameInstance.h"
#include "../LobbyGameMode.h"

#include "JustAnotherLoobyBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UJustAnotherLoobyBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	/** Show the native loading screen, such as on a map transfer. If bPlayUntilStopped is false, it will be displayed for PlayTime and automatically stop */
	UFUNCTION(BlueprintCallable, Category = "Loading", meta = (WorldContext = "WorldContextObject"))
	static void ShowLoadingScreen(const UObject* WorldContextObject, bool bPlayUntilStopped, float PlayTime);

	/** Turns off the native loading screen if it is visible. This must be called if bPlayUntilStopped was true */
	UFUNCTION(BlueprintCallable, Category = "Loading", meta = (WorldContext = "WorldContextObject"))
	static void HideLoadingScreen(const UObject* WorldContextObject);

	/** Return the JustAnotherLobbyGameIsntance */
	UFUNCTION(BlueprintPure, Category = "Game Instance", meta = (WorldContext = "WorldContextObject"))
	static UJustAnotherLobbyGameInstance* GetJustAnotherLobbyGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Game Mode", meta = (WorldContext = "WorldContextObject"))
	static ALobbyGameMode* GetLobbyGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "UI", meta = (WorldContext = "WorldContextObject"))
	static UUserWidget* CreateAndShowWidget(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass, bool bShowMouseCursor, bool bOnlyCreate);

	UFUNCTION(BlueprintCallable, Category = "Common", meta = (WorldContext = "WorldContextObject"))
	static bool IsServer(UObject* WorldContextObject);
};
