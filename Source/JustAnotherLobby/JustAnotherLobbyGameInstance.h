// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "MoviePlayer.h"

#include "Common/Struct/Heroes.h"
#include "Common/Struct/ConfigurationMaps.h"
#include "Common/Struct/LobbyHeroeSpot.h"
#include "Common/Struct/InGamePlayerInfo.h"

#include "Characters/CharacterBase.h"
#include "UI/MainMenu/MainMenu.h"

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

    /* Main Menu */

    UPROPERTY(EditAnywhere)
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

    /* General Settings */

    UPROPERTY(Replicated)
    int32 MaxPlayers;

    UPROPERTY(Replicated)
    FString ServerName;

    UPROPERTY(EditAnywhere)
    TArray<FHeroes> Heroes;
    TArray<FInGamePlayerInfo> InGamePlayersInfo;
    UPROPERTY(EditAnywhere)
    UTexture2D* LoadingScreenImage;

    UPROPERTY(EditAnywhere)
    TMap<FString, FConfigurationMaps> ConfigurationMaps;

    UPROPERTY(EditAnywhere)
    TMap<int32, FLobbyHeroeSpot> ConfigurationLobbyHeroeSpot;

    UFUNCTION(BlueprintCallable)
    void SetHostSettings(int32 InNumberOfPlayers, FString InServerName);

    UFUNCTION(BlueprintCallable)
    void PlayEnviromentMusic(USoundBase* Audio, float Volume, bool bIsPersistLevel);

    UFUNCTION(BlueprintCallable)
    void StopEnviromentMusic();

    UFUNCTION(BlueprintCallable)
    bool GetIsFirstTimeLoading();

    UFUNCTION(BlueprintCallable)
    void SetFirstTimeLoading(bool InbIsFirstTimeLoading);

    UFUNCTION()
    TSubclassOf<ACharacterBase> GetHeroeByName(FString InHeroeName);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
    UAudioComponent* Music;

    class UMainMenu* Menu;
    FLoadingScreenAttributes LoadingScreenAttributes;
};