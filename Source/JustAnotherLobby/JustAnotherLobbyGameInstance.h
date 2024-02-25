// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/LoadingScreen/LoadingScreen.h"
#include "./UI/LoadingScreen/LoadingScreen.h"

#include "JustAnotherLobbyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UJustAnotherLobbyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);

	UJustAnotherLobbyGameInstance(const FObjectInitializer& ObjectInitializer);

    void Init() override;
    
    UPROPERTY(EditAnyWhere)
    TSubclassOf<ULoadingScreen> LoadingScreenClass;

    UFUNCTION(BlueprintCallable)
    void SetHostGame(bool InbIsHostGameMenu);

    UFUNCTION(BlueprintCallable)
    void SetFindGames(bool InbIsFindGamesMenu);

    UFUNCTION(BlueprintCallable)
    void SetOptionsMenu(bool InbIsOptionsMenu);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Loading Screen")
    void BeginLoadingScreen(const FString& MapName);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Loading Screen")
    void EndLoadingScreen(UWorld* InLoadedWorld);

    UFUNCTION(BlueprintCallable)
    void ShowLoadingScreen(bool bWithTransition);

    UFUNCTION(BlueprintCallable)
    void HideLoadingScreen(bool bWithTransition);

    UFUNCTION(BlueprintCallable)
    void OpenNextLevel(FName InLevel, bool bIsListen, bool bShowLoading, float InOpenLevelDelay);


    

private:
    UPROPERTY()
    bool bIsHostGameMenu{ false };
    UPROPERTY()
    bool bIsFindGamesMenu{ false };
    UPROPERTY()
    bool bIsOptionsMenu{ false };
    UPROPERTY()
    ULoadingScreen* LoadingScreen;

    UFUNCTION()
    void OpenLevelWithDelay(FName InLevelName, FString InListen);
};


