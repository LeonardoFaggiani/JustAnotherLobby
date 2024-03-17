// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/LoadingScreen/LoadingScreen.h"
#include "UI/MainMenu/MainMenu.h"
#include "./UI/Struct/Heroes.h"
#include "./UI/Struct/ConfigurationMaps.h"
#include "./UI/Struct/LobbyHeroeSpot.h"
#include "./UI/LoadingScreen/LoadingScreen.h"
#include <MoviePlayer/Public/MoviePlayer.h>
#include "UI/Struct/InGamePlayerInfo.h"

#include "JustAnotherLobbyGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UJustAnotherLobbyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    
    UJustAnotherLobbyGameInstance(const FObjectInitializer& ObjectInitializer);

    void Init() override;
    
    void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);


    /* Main Menu */

    UPROPERTY(EditAnyWhere)
    TSubclassOf<UMainMenu> MenuClass;

    UFUNCTION(BlueprintCallable)
    UMainMenu* LoadMenu();

    UFUNCTION(BlueprintCallable)
    void SetHostGame(bool InbIsHostGameMenu);

    UFUNCTION(BlueprintCallable)
    void SetFindGames(bool InbIsFindGamesMenu);

    UFUNCTION(BlueprintCallable)
    void SetOptionsMenu(bool InbIsOptionsMenu);

    UFUNCTION(BlueprintCallable)
    void SetBackToMainMenu(bool InbIsBackToMainMenu);

    UFUNCTION(BlueprintCallable)
    bool GetHostGame();

    UFUNCTION(BlueprintCallable)
    bool GetFindGames();

    UFUNCTION(BlueprintCallable)
    bool GetOptionsMenu();

    UFUNCTION(BlueprintCallable)
    bool GetBackToMainMenu();

    /* End Main Menu */


    /* Loading Screen */

    UPROPERTY(EditAnyWhere)
    TSubclassOf<ULoadingScreen> LoadingScreenClass;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Loading Screen")
    void BeginLoadingScreen(const FString& MapName);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Loading Screen")
    void EndLoadingScreen(UWorld* InLoadedWorld);

    UFUNCTION(BlueprintCallable)
    void ShowLoadingScreen(bool bWithTransition);

    UFUNCTION(BlueprintCallable)
    void HideLoadingScreen(bool bWithTransition);

    /* End Loading Screen */


    /* General Settings */

    UPROPERTY(Replicated)
    int32 MaxPlayers;
    UPROPERTY(Replicated)
    FString ServerName;

    UPROPERTY(EditAnyWhere)
    TArray<FHeroes> Heroes;
    TArray<FInGamePlayerInfo> InGamePlayersInfo;

    UPROPERTY(EditAnyWhere)
    TMap<FString, FConfigurationMaps> ConfigurationMaps;

    UPROPERTY(EditAnyWhere)
    TMap<int32, FLobbyHeroeSpot> ConfigurationLobbyHeroeSpot;

    UFUNCTION(BlueprintCallable)
    void SetHostSettings(int32 InNumberOfPlayers, FString InServerName);

    UFUNCTION(BlueprintCallable)
    void PlayEnviromentMusic(USoundBase* Audio, float Volume, bool bIsPersistLevel);

    UFUNCTION(BlueprintCallable)
    void StopEnviromentMusic();

    UFUNCTION(BlueprintCallable)
    void OpenNextLevel(FName InLevel, bool bIsListen, bool bShowLoading, float InOpenLevelDelay);

    UFUNCTION(BlueprintCallable)
    bool GetIsFirstTimeLoading();

    UFUNCTION(BlueprintCallable)
    void SetFirstTimeLoading(bool InbIsFirstTimeLoading);

    UFUNCTION()
    TSubclassOf<ACharacterBase> GetHeroeByName(FString InHeroeName);

    /* End General Settings */    

private:
    UPROPERTY()
    bool bIsHostGameMenu{ false };
    UPROPERTY()
    bool bIsFindGamesMenu{ false };
    UPROPERTY()
    bool bIsOptionsMenu{ false };
    UPROPERTY()
    bool bIsBackToMainMenu{ false };
    UPROPERTY()
    bool bIsFirstTimeLoading{ true };
    UPROPERTY()
    ULoadingScreen* LoadingScreen;
    UPROPERTY()
    UAudioComponent* Music;

    UFUNCTION()
    void OpenLevelWithDelay(FName InLevelName, FString InListen);

    class UMainMenu* Menu;
    FLoadingScreenAttributes LoadingScreenAttributes;
};