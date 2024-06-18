// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/Widget.h"
#include "JustAnotherLobbyGameInstance.h"
#include "UI/Struct/LobbyPlayerInfo.h"
#include "UI/Struct/LobbyHeroeSpot.h"
#include "UI/Struct/PlayerKickNameIndex.h"
#include "UI/Lobby/Lobby.h"
#include "UI/Lobby/HeroeSelection.h"
#include "LobbyPlayerController.generated.h"

class ACharacterBase;

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
	void SetSubclassHeroeSelected(TSubclassOf<ACharacterBase> InSubclassHeroeSelected);
	TSubclassOf<ACharacterBase> GetSubclassHeroeSelected();
	void SetLobbyHeroeSpot(const FLobbyHeroeSpot& InLobbyHeroeSpot);
	FLobbyHeroeSpot GetLobbyHeroeSpot();
	void SetPlayerIndex(int32 InIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* CommonPlayerControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChatWindowAction;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<ULobby> LobbyClass;

	UPROPERTY(BlueprintReadWrite, Replicated)
	FLobbyPlayerInfo PlayerSettings;

	UPROPERTY(BlueprintReadWrite)
	FLobbyHeroeSpot LobbyHeroeSpot;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CallUpdate(const FLobbyPlayerInfo& PlayerInfo);
	void Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_NotifyPlayerStatus(const FLobbyPlayerInfo& PlayerInfo);
	void Server_NotifyPlayerStatus_Implementation(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_FillContainerPlayerKickList();
	void Server_FillContainerPlayerKickList_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SubmitChat(const FText& InPlayerName, const FText& InMessage);
	void Server_SubmitChat_Implementation(const FText& InPlayerName, const FText& InMessage);

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

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_UpdateChat(const FText& InPlayerName, const FText& InMessage);
	void Client_UpdateChat_Implementation(const FText& InPlayerName, const FText& InMessage);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_FillContainerPlayerKickList(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex);
	void Client_FillContainerPlayerKickList_Implementation(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multi_UpdateReadyStatusInPlayerKickList(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex);
	void Multi_UpdateReadyStatusInPlayerKickList_Implementation(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex);

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void SetFocusOnChatWindow(const FInputActionValue& Value);
	class ULobby* Lobby;
	class UHeroeSelection* HeroeSelection;
	class ALobbyGameMode* LobbyGameMode;
	class UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
	AActor* ViewTarget;
	AActor* GetActorByName(FString InActorName);	
	ACharacterBase* CurrentCharacter;
	TSubclassOf<ACharacterBase> SubclassHeroeSelected;
};
