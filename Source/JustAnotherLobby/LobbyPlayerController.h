// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/Widget.h"
#include "JustAnotherLobbyGameInstance.h"
#include "UI/Struct/LobbyPlayerInfo.h"
#include "UI/Lobby/Lobby.h"
#include "UI/Lobby/HeroeSelection.h"
#include "LobbyPlayerController.generated.h"

class ACharacterBase;
class ALobbyPlayerSpots;

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);

	void SetCurrentCharacter(ACharacterBase* InCurrentCharacter);
	ACharacterBase* GetCurrentCharacter();
	void SetPlayerSpot(ALobbyPlayerSpots* InPlayerSpot);
	ALobbyPlayerSpots* GetPlayerSpot();

	UPROPERTY(EditAnyWhere)
	TSubclassOf<ULobby> LobbyClass;

	UPROPERTY(BlueprintReadWrite, Replicated)
	FLobbyPlayerInfo PlayerSettings;

	UPROPERTY(BlueprintReadWrite, Replicated)
	ALobbyPlayerSpots* PlayerSpot;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CallUpdate(const FLobbyPlayerInfo& PlayerInfo);
	void Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_NotifyPlayerStatus(const FLobbyPlayerInfo& PlayerInfo);
	void Server_NotifyPlayerStatus_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SetupLobbyMenu(const FString& ServerName);
	void Client_SetupLobbyMenu_Implementation(const FString& ServerName);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_UpdateLobbySettings(UTexture2D* MapImage, const FString& MapName);
	void Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_UpdateNumberOfPlayers(int32 CurrentPlayers, int32 MaxPlayers);
	void Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_AssignHeroeToPlayer(TSubclassOf<ACharacterBase> HeroeClass);
	void Client_AssignHeroeToPlayer_Implementation(TSubclassOf<ACharacterBase> HeroeClass);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ShowLoadingScreen();
	void Client_ShowLoadingScreen_Implementation();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SetViewTargetSpot();
	void Client_SetViewTargetSpot_Implementation();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SwitchToLobbyMode();
	void Client_SwitchToLobbyMode_Implementation();

private:
	class ULobby* Lobby;
	class UHeroeSelection* HeroeSelection;
	class ALobbyGameMode* LobbyGameMode;
	class UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
	AActor* ViewTarget;
	AActor* GetActorByName(FString InActorName);
	ACharacterBase* CurrentCharacter;
};
