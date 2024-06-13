// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Characters/CharacterBase.h"
#include "UI/Struct/LobbyHeroeSpot.h"
#include "UI/Lobby/LobbyPlayerSpots.h"
#include "LobbyPlayerController.h"
#include "JustAnotherLobbyGameInstance.h"

#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode(const FObjectInitializer& ObjectInitializer);
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	void LaunchTheGame();

	bool IsAllPlayerReady();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SwapCharacter(APlayerController* PlayerController, TSubclassOf<ACharacterBase> InHeroeSelected, bool bChangeStatus);
	void Server_SwapCharacter_Implementation(APlayerController* PlayerController, TSubclassOf<ACharacterBase> InHeroeSelected, bool bChangeStatus);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UpdateGameSettings(UTexture2D* mapImage, const FString& mapName);
	void Server_UpdateGameSettings_Implementation(UTexture2D* mapImage, const FString& mapName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RespawnPlayer(ALobbyPlayerController* LobbyPlayerController);
	void Server_RespawnPlayer_Implementation(ALobbyPlayerController* LobbyPlayerController);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_EveryoneUpdate();
	void Server_EveryoneUpdate_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnLobbyPlayerSpot(ALobbyPlayerController* LobbyPlayerController);
	void Server_SpawnLobbyPlayerSpot_Implementation(ALobbyPlayerController* LobbyPlayerController);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UpdatePlayerName();
	void Server_UpdatePlayerName_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetViewTargetSpot();
	void Server_SetViewTargetSpot_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_FillContainerPlayerKickList();
	void Server_FillContainerPlayerKickList_Implementation();
	

	FLobbyHeroeSpot* GetLobbyHeroeSpotByPlayerConnected(ALobbyPlayerController* LobbyPlayerController);

	UPROPERTY(BlueprintReadOnly, Replicated)
		TArray<class ALobbyPlayerController*> AllPlayerControllers;
	UPROPERTY(Replicated)
		TArray<struct FLobbyPlayerInfo> ConnectedPlayers;
	UPROPERTY(Replicated)
		UTexture2D* MapImage;
	UPROPERTY(Replicated)
		FString MapName;
	UPROPERTY(Replicated)
		int32 CurrentPlayers;
	UPROPERTY(Replicated)
		int32 MaxPlayers;
		TSubclassOf<ACharacterBase> HeroeDefault;
	UPROPERTY(EditAnyWhere)
		TSubclassOf<ALobbyPlayerSpots> LobbyPlayerSpotClass;

private:
	void DestroyCharacterSelectedIfExits(ALobbyPlayerController* LobbyPlayerController);
	void SpawnCharacterOnPlayerSpot(ALobbyPlayerController* LobbyPlayerController);
	void UpdatePlayerName(ALobbyPlayerController* LobbyPlayerController);
	UFUNCTION()
	void UpdateReadyStatus(ALobbyPlayerController* LobbyPlayerController);
	void FillConnectedPlayers();
	void SetPlayerInfoToTransfer();
	TArray<FPlayerKickNameIndex> GetPlayerKickNameIndex();
	class UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
};
