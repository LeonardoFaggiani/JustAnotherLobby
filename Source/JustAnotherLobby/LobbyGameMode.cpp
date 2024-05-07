// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "JustAnotherLobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UI/Struct/InGamePlayerInfo.h"


ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		UGameInstance* GameInstance = World->GetGameInstance();

		if (IsValid(GameInstance)) {
			this->JustAnotherLobbyGameInstance = Cast<UJustAnotherLobbyGameInstance>(GameInstance);
		}
	}
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
		this->AllPlayerControllers.Remove(LobbyPlayerController);

	Server_EveryoneUpdate();
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

		this->HeroeDefault = InHeroeSelected;

		this->DestroyCharacterSelectedIfExits(LobbyPlayerController);
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

void ALobbyGameMode::LaunchTheGame()
{
	UWorld* World = GetWorld();

	const FString FullMapPath = FString(TEXT("/Game/Maps/{0}?listen"));

	FString MapToTravel = FString::Format(*FullMapPath, { MapName });

	if (World) {
		//bUseSeamlessTravel = true;	
		this->SetPlayerInfoToTransfer();
		World->ServerTravel(FString(MapToTravel));
	}
}

void ALobbyGameMode::SpawnCharacterOnPlayerSpot(ALobbyPlayerController* LobbyPlayerController)
{
	FLobbyHeroeSpot* LobbyHeroeSpotByIndex = this->GetLobbyHeroeSpotByPlayerConnected(LobbyPlayerController);

	if (!IsValid(this->HeroeDefault))
		return;

	FActorSpawnParameters params;

	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (LobbyHeroeSpotByIndex != nullptr)
	{
		ACharacterBase* SpawnCharacter = Cast<ACharacterBase>(GetWorld()->SpawnActor<ACharacterBase>(this->HeroeDefault, LobbyHeroeSpotByIndex->LocationHeroe.Location, LobbyHeroeSpotByIndex->LocationHeroe.Rotation, params));
		SpawnCharacter->Multi_PlayStartLevelMontage();

		LobbyPlayerController->SetCurrentCharacter(SpawnCharacter);
		LobbyPlayerController->PlayerSettings.HeroeSelected = this->HeroeDefault;		

		UE_LOG(LogTemp, Warning, TEXT("SpawnCharacterOnPlayerSpot Fired!"));
	}

	this->UpdatePlayerName(LobbyPlayerController);
}

void ALobbyGameMode::DestroyCharacterSelectedIfExits(ALobbyPlayerController* LobbyPlayerController)
{
	ACharacterBase* CharacterBase = LobbyPlayerController->GetCurrentCharacter();

	if (IsValid(CharacterBase))
		CharacterBase->Destroy();

	this->SpawnCharacterOnPlayerSpot(LobbyPlayerController);
}

void ALobbyGameMode::Server_RespawnPlayer_Implementation(ALobbyPlayerController* LobbyPlayerController)
{
	this->DestroyCharacterSelectedIfExits(LobbyPlayerController);
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

	if (IsValid(CharacterBase)) {
		CharacterBase->Multi_SetReadyStatus(LobbyPlayerController->PlayerSettings.bPlayerReadyState);
	}
}

void ALobbyGameMode::FillConnectedPlayers()
{
	this->CurrentPlayers = this->AllPlayerControllers.Num();

	if (this->CurrentPlayers > 0)
		this->ConnectedPlayers.Empty();

	for (ALobbyPlayerController* LobbyPlayerController : this->AllPlayerControllers)
	{
		this->ConnectedPlayers.Add(LobbyPlayerController->PlayerSettings);
	}
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