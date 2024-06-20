// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../JustAnotherLobbyGameInstance.h"
#include "../PlayerController/InGamePlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "InGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API AInGameGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AInGameGameMode(const FObjectInitializer& ObjectInitializer);
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(Replicated)
	TArray<struct FInGamePlayerInfo> ConnectedPlayers;
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<class AInGamePlayerController*> AllPlayerControllers;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RespawnPlayer(AInGamePlayerController* InGamePlayerController);
	void Server_RespawnPlayer_Implementation(AInGamePlayerController* InGamePlayerController);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UpdatePlayerName();
	void Server_UpdatePlayerName_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetOverheadPlayer(bool bIsHiddenIcon, const FString& ColorName);
	void Server_SetOverheadPlayer_Implementation(bool bIsHiddenIcon, const FString& ColorName);
private:
	UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
	TSubclassOf<APlayerStart> PlayerStart;

private:
	void UpdatePlayerName(AInGamePlayerController* InGamePlayerController);
	TArray<AActor*> GetAllPlayersStart();
	FTransform GetTransformOfPlayerStartByIndex(int32 InPlayerIndex);
	FInGamePlayerInfo* GetPlayerInfoByIndex(int32 InPlayerIndex);
};
