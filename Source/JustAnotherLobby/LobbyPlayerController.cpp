// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "./UI/Lobby/OverheadPlayerSpot.h"
#include "Library/JustAnotherLoobyBlueprintLibrary.h"
#include "Components/Widget.h"
#include "Common/FLogJustAnotherLobby.h"
#include "Net/UnrealNetwork.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UWorld* World = this->GetWorld();

	if (IsValid(World))
	{
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);

		if (IsValid(GameMode))
			this->LobbyGameMode = Cast<ALobbyGameMode>(GameMode);

		this->JustAnotherLobbyGameInstance = UJustAnotherLoobyBlueprintLibrary::GetJustAnotherLobbyGameInstance(this);
	}
}

#pragma region Server

void ALobbyPlayerController::Server_CallUpdate_Implementation(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerSettings = PlayerInfo;

	if (IsValid(this->LobbyGameMode)) {
		this->LobbyGameMode->Server_SwapCharacter(this, PlayerSettings.HeroeSelected, PlayerSettings.bPlayerReadyState);
		this->LobbyGameMode->Server_EveryoneUpdate();
	}
}

void ALobbyPlayerController::Server_NotifyPlayerStatus_Implementation(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerSettings = PlayerInfo;

	if (IsValid(this->LobbyGameMode))
		this->LobbyGameMode->Server_EveryoneUpdate();
}

void ALobbyPlayerController::Server_FillContainerPlayerKickList_Implementation()
{	
	if (IsValid(this->LobbyGameMode))
		this->LobbyGameMode->Server_FillContainerPlayerKickList();	
}

#pragma endregion Server

#pragma region Client

void ALobbyPlayerController::Client_SetupLobbyMenu_Implementation(const FString& ServerName)
{
	if (!ensure(this->LobbyClass != nullptr)) return;

	this->Lobby = CreateWidget<ULobby>(this, this->LobbyClass);
	this->Lobby->SetServerName(ServerName);
	this->Lobby->Setup();
}

void ALobbyPlayerController::Client_UpdateLobbySettings_Implementation(UTexture2D* MapImage, const FString& MapName)
{
	if (!IsValid(this->Lobby)) return;

	this->Lobby->SetMap(MapImage, MapName);
}

void ALobbyPlayerController::Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers, int32 MaxPlayers)
{
	if (!IsValid(this->Lobby)) return;

	TArray<FStringFormatArg> CurrentAndMaxPlayers;
	CurrentAndMaxPlayers.Add(FStringFormatArg(CurrentPlayers));
	CurrentAndMaxPlayers.Add(FStringFormatArg(MaxPlayers));

	FString CurrentAndMaxPlayersFormat = FString::Format(TEXT("{0}/{1} players"), CurrentAndMaxPlayers);

	if (IsValid(this->LobbyGameMode))
		this->Lobby->SetEnablePlayButton(this->LobbyGameMode->IsAllPlayerReady());

	this->Lobby->SetCurrentPlayersFormat(CurrentAndMaxPlayersFormat);
}

void ALobbyPlayerController::Client_AssignHeroeToPlayer_Implementation(TSubclassOf<ACharacterBase> HeroeClass)
{
	if (this->PlayerSettings.bPlayerReadyState)
		return;

	this->PlayerSettings.HeroeSelected = HeroeClass;

	this->Server_CallUpdate(this->PlayerSettings);
}

void ALobbyPlayerController::Client_ShowLoadingScreen_Implementation()
{
	UJustAnotherLoobyBlueprintLibrary::ShowLoadingScreen(this, true, 3);
}

void ALobbyPlayerController::Client_SetViewTargetSpot_Implementation()
{
	this->ViewTarget = GetActorByName("ViewTargetSpot");

	this->SetViewTargetWithBlend(this->ViewTarget);
}

void ALobbyPlayerController::Client_SwitchToLobbyMode_Implementation()
{
	if (!IsValid(this->Lobby)) return;

	this->Lobby->SetHiddenHeroesButton(false);

	this->Lobby->ReadyButton->SetIsEnabled(true);
}

void ALobbyPlayerController::Client_FillContainerPlayerKickList_Implementation(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex)
{
	this->Lobby->FillContainerPlayerKickList(InPlayerNamesIndex);
}

void ALobbyPlayerController::Multi_UpdateReadyStatusInPlayerKickList_Implementation(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex)
{
	if (!IsValid(this->Lobby)) return;

	this->Lobby->UpdateReadyStatusInPLayerKickList(InPlayerNamesIndex);
}

#pragma endregion Client


void ALobbyPlayerController::SetCurrentCharacter(ACharacterBase* InCurrentCharacter)
{
	this->CurrentCharacter = InCurrentCharacter;
}

ACharacterBase* ALobbyPlayerController::GetCurrentCharacter()
{
	return this->CurrentCharacter;
}

void ALobbyPlayerController::SetSubclassHeroeSelected(TSubclassOf<ACharacterBase> InSubclassHeroeSelected)
{
	this->SubclassHeroeSelected = InSubclassHeroeSelected;
}

TSubclassOf<ACharacterBase> ALobbyPlayerController::GetSubclassHeroeSelected()
{
	return this->SubclassHeroeSelected;
}

void ALobbyPlayerController::SetLobbyHeroeSpot(const FLobbyHeroeSpot& InLobbyHeroeSpot)
{
	this->LobbyHeroeSpot = InLobbyHeroeSpot;
}

void ALobbyPlayerController::SetPlayerIndex(int32 InIndex)
{
	this->PlayerSettings.PlayerIndex = InIndex;
}

FLobbyHeroeSpot ALobbyPlayerController::GetLobbyHeroeSpot()
{
	return this->LobbyHeroeSpot;
}

AActor* ALobbyPlayerController::GetActorByName(FString InActorName)
{
	TArray<AActor*> ActorsInScene{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsInScene);

	if (ActorsInScene.Num() > 0)
	{
		for (AActor* actor : ActorsInScene)
		{
			if (actor == nullptr)
				return nullptr;

			if (actor->GetActorNameOrLabel() == InActorName)
				return actor;
		}
	}

	return nullptr;
}


void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerSettings);
	//DOREPLIFETIME(ALobbyPlayerController, PlayerSpot);
}