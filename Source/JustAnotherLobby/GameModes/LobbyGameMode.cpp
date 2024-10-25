// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "../PlayerState/JustAnotherLobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../Common/Library/JustAnotherLoobyBlueprintLibrary.h"
#include "../Common/Struct/PlayerKickNameIndex.h"
#include "../Common/Struct/InGamePlayerInfo.h"

ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	this->JustAnotherLobbyGameInstance = UJustAnotherLoobyBlueprintLibrary::GetJustAnotherLobbyGameInstance(this);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {

	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		//if the joining player is a lobby player controller, add him to a list of connected Players
		if (NewPlayer) {

			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayer);

			this->AllPlayerControllers.Add(LobbyPlayerController);

			LobbyPlayerController->SetPlayerIndex(this->AllPlayerControllers.Num());

			if (this->JustAnotherLobbyGameInstance != nullptr) {

				MaxPlayers = this->JustAnotherLobbyGameInstance->MaxPlayers;

				FConfigurationMaps* DefaultMap = this->JustAnotherLobbyGameInstance->ConfigurationMaps.Find(TEXT("Cementery"));

				UTexture2D* mapImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *DefaultMap->ImagePath));

				LobbyPlayerController->Client_SetupLobbyMenu(this->JustAnotherLobbyGameInstance->ServerName);

				LobbyPlayerController->Client_UpdateLobbySettings(mapImage, *DefaultMap->Name);

				Server_SpawnLobbyPlayerSpot(LobbyPlayerController);

				Server_RespawnPlayer(LobbyPlayerController);

				Server_EveryoneUpdate();

				Server_FillContainerPlayerKickList();

				FTimerHandle MemberTimerHandle;

				GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &ALobbyGameMode::Server_SetViewTargetSpot, 0.2f, false);
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting) {

	Super::Logout(Exiting);

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting);

	if (LobbyPlayerController)
		Server_RemoveCharacterFromLobby(LobbyPlayerController);	
	
	Server_EveryoneUpdate();
	Server_FillContainerPlayerKickList();	
}

bool ALobbyGameMode::IsAllPlayerReady()
{
	for (FLobbyPlayerInfo PlayerInfo : this->ConnectedPlayers)
	{
		if (!PlayerInfo.bPlayerReadyState)
			return false;
	}

	return true;
}

void ALobbyGameMode::Server_SwapCharacter_Implementation(APlayerController* PlayerController, TSubclassOf<ACharacterBase> InHeroeSelected, bool bChangeStatus)
{
    if (!bChangeStatus) {

        ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);

		this->DestroyCharacterSelectedIfExits(LobbyPlayerController);

        LobbyPlayerController->SetCurrentCharacter(InHeroeSelected.GetDefaultObject());
        LobbyPlayerController->SetSubclassHeroeSelected(InHeroeSelected);

        this->SpawnCharacterOnPlayerSpot(LobbyPlayerController);
    }
}

void ALobbyGameMode::Server_UpdateGameSettings_Implementation(UTexture2D* mapImage, const FString& mapName)
{
	this->MapImage = mapImage;
	this->MapName = mapName;

	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)	
		PlayerController->Client_UpdateLobbySettings(this->MapImage, this->MapName);	
}

void ALobbyGameMode::Server_SetViewTargetSpot_Implementation()
{
	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)	
		PlayerController->Client_SetViewTargetSpot();
	
}

void ALobbyGameMode::Server_SpawnLobbyPlayerSpot_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
    FLobbyHeroeSpot* LobbyHeroeSpotByIndex = this->GetLobbyHeroeSpotByPlayerConnected(LobbyPlayerController);

    if (LobbyHeroeSpotByIndex != nullptr)
    {
        FActorSpawnParameters params;

        params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)        
            ALobbyPlayerSpots* LobbyPlayerSpotSpawned = Cast<ALobbyPlayerSpots>(GetWorld()->SpawnActor<ALobbyPlayerSpots>(LobbyPlayerSpotClass, LobbyHeroeSpotByIndex->LocationSpot.Location, LobbyHeroeSpotByIndex->LocationSpot.Rotation, params));
    }
}

void ALobbyGameMode::Server_UpdatePlayerName_Implementation()
{
	for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)
		this->UpdatePlayerName(LobbyPlayerController);
}

void ALobbyGameMode::Server_EveryoneUpdate_Implementation()
{
	this->FillConnectedPlayers();

	for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)
	{
		FTimerHandle MemberTimerHandle;
		FTimerDelegate TimerDel;

		TimerDel.BindUFunction(this, FName("UpdateReadyStatus"), LobbyPlayerController);

		GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, TimerDel, 0.1f, false);

		LobbyPlayerController->Client_UpdateNumberOfPlayers(this->CurrentPlayers, this->MaxPlayers);
	}
}

void ALobbyGameMode::Server_FillContainerPlayerKickList_Implementation()
{
    if (this->AllPlayerControllers.Num() > 0)
    {
		TArray<FPlayerKickNameIndex> PlayerNamesIndex = GetPlayerKickNameIndex();      

		for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)
			PlayerController->Client_FillContainerPlayerKickList(PlayerNamesIndex);					   
    }
}

void ALobbyGameMode::Server_RemoveCharacterFromLobby_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
	this->DestroyCharacterSelectedIfExits(LobbyPlayerController);

	this->AllPlayerControllers.Remove(LobbyPlayerController);
}

void ALobbyGameMode::Server_LaunchTheGame_Implementation()
{
	UWorld* World = GetWorld();

	const FString FullMapPath = FString(TEXT("/Game/Maps/{0}?listen"));

	FString MapToTravel = FString::Format(*FullMapPath, { MapName });

	if (World) {		
		this->SetPlayerInfoToTransfer();
		World->ServerTravel(FString(MapToTravel));
	}
}

void ALobbyGameMode::SpawnCharacterOnPlayerSpot(ALobbyPlayerController* LobbyPlayerController)
{
	FLobbyHeroeSpot* LobbyHeroeSpotByIndex = this->GetLobbyHeroeSpotByPlayerConnected(LobbyPlayerController);

	if (!IsValid(LobbyPlayerController->GetSubclassHeroeSelected()))
		return;

	FActorSpawnParameters params;

	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (LobbyHeroeSpotByIndex != nullptr)
	{
		ACharacterBase* SpawnCharacter = Cast<ACharacterBase>(GetWorld()->SpawnActor<ACharacterBase>(LobbyPlayerController->GetSubclassHeroeSelected(), LobbyHeroeSpotByIndex->LocationHeroe.Location, LobbyHeroeSpotByIndex->LocationHeroe.Rotation, params));

		LobbyPlayerController->SetCurrentCharacter(SpawnCharacter);

		SpawnCharacter->Multi_PlayStartLevelMontage();

		LobbyPlayerController->PlayerSettings.HeroeSelected = LobbyPlayerController->GetSubclassHeroeSelected();
	}

	this->UpdatePlayerName(LobbyPlayerController);
}

void ALobbyGameMode::DestroyCharacterSelectedIfExits(ALobbyPlayerController* LobbyPlayerController)
{
	ACharacterBase* CharacterBase = LobbyPlayerController->GetCurrentCharacter();

	if (IsValid(CharacterBase))
		CharacterBase->Destroy();
}

void ALobbyGameMode::Server_RespawnPlayer_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
	this->SpawnCharacterOnPlayerSpot(LobbyPlayerController);
}

void ALobbyGameMode::Server_ShouldHideLoadingScreen_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
    if (this->AllPlayerControllers.Contains(LobbyPlayerController)) {
        UJustAnotherLoobyBlueprintLibrary::HideLoadingScreen(this);
    }
}

void ALobbyGameMode::UpdatePlayerName(ALobbyPlayerController* LobbyPlayerController)
{
	AJustAnotherLobbyPlayerState* JustAnotherLobbyPlayerState = LobbyPlayerController->GetPlayerState<AJustAnotherLobbyPlayerState>();

	ACharacterBase* CharacterBase = LobbyPlayerController->GetCurrentCharacter();

	if (IsValid(CharacterBase) && IsValid(JustAnotherLobbyPlayerState)) {

		LobbyPlayerController->PlayerSettings.PlayerName = JustAnotherLobbyPlayerState->GetPlayerName();		

		CharacterBase->Multi_SetPlayerName(LobbyPlayerController->PlayerSettings.PlayerName);
	}
}

void ALobbyGameMode::UpdateReadyStatus(ALobbyPlayerController* LobbyPlayerController)
{
	ACharacterBase* CharacterBase = LobbyPlayerController->GetCurrentCharacter();

	if (IsValid(CharacterBase))
		CharacterBase->Multi_SetReadyStatus(LobbyPlayerController->PlayerSettings.bPlayerReadyState);	

	TArray<FPlayerKickNameIndex> PlayerNamesIndex = GetPlayerKickNameIndex();

	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers)
		PlayerController->Multi_UpdateReadyStatusInPlayerKickList(PlayerNamesIndex);	
}

void ALobbyGameMode::FillConnectedPlayers()
{
	this->CurrentPlayers = this->AllPlayerControllers.Num();

	if (this->CurrentPlayers > 0)
		this->ConnectedPlayers.Empty();

	for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)	
		this->ConnectedPlayers.Add(LobbyPlayerController->PlayerSettings);	
}

void ALobbyGameMode::SetPlayerInfoToTransfer()
{
	for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)
	{
		FInGamePlayerInfo InGamePlayerInfo;

		InGamePlayerInfo.HeroeSelected = LobbyPlayerController->PlayerSettings.HeroeSelected;
		InGamePlayerInfo.PlayerName = LobbyPlayerController->PlayerSettings.PlayerName;
		InGamePlayerInfo.PlayerIndex = LobbyPlayerController->PlayerSettings.PlayerIndex;

		this->JustAnotherLobbyGameInstance->InGamePlayersInfo.Add(InGamePlayerInfo);
	}
}

TArray<FPlayerKickNameIndex> ALobbyGameMode::GetPlayerKickNameIndex()
{
	TArray<FPlayerKickNameIndex> PlayerNamesIndex{};

	for (ALobbyPlayerController* PlayerController : this->AllPlayerControllers) {

		AJustAnotherLobbyPlayerState* JustAnotherLobbyPlayerState = PlayerController->GetPlayerState<AJustAnotherLobbyPlayerState>();

		if (IsValid(JustAnotherLobbyPlayerState)) {

			FPlayerKickNameIndex PlayerKickNameIndex;

			PlayerKickNameIndex.Name = FText::FromString(JustAnotherLobbyPlayerState->GetPlayerName());
			PlayerKickNameIndex.Index = this->AllPlayerControllers.Find(PlayerController);
			PlayerKickNameIndex.IsReady = PlayerController->PlayerSettings.bPlayerReadyState;

			PlayerNamesIndex.Add(PlayerKickNameIndex);
		}
	}

	return PlayerNamesIndex;
}

FLobbyHeroeSpot* ALobbyGameMode::GetLobbyHeroeSpotByPlayerConnected(ALobbyPlayerController* LobbyPlayerController)
{
	FLobbyHeroeSpot* LobbyHeroeSpot = this->JustAnotherLobbyGameInstance->ConfigurationLobbyHeroeSpot.Find(LobbyPlayerController->PlayerSettings.PlayerIndex);

	return LobbyHeroeSpot;
}

void ALobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameMode, AllPlayerControllers);
	DOREPLIFETIME(ALobbyGameMode, MapImage);
	DOREPLIFETIME(ALobbyGameMode, MapName);
	DOREPLIFETIME(ALobbyGameMode, CurrentPlayers);
	DOREPLIFETIME(ALobbyGameMode, MaxPlayers);
	DOREPLIFETIME(ALobbyGameMode, ConnectedPlayers);
}