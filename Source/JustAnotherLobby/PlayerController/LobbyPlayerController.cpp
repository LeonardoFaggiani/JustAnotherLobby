// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "../GameModes/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Widget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "../UI/Lobby/OverheadPlayerSpot.h"
#include "../Common/Library/JustAnotherLoobyBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->LobbyGameMode = UJustAnotherLoobyBlueprintLibrary::GetLobbyGameMode(this);
	this->JustAnotherLobbyGameInstance = UJustAnotherLoobyBlueprintLibrary::GetJustAnotherLobbyGameInstance(this);	
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
		
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))	
		Subsystem->AddMappingContext(this->CommonPlayerControllerMappingContext, 0);	

	FInputModeGameAndUI FInputModeGameAndUI;

	this->SetInputMode(FInputModeGameAndUI);
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(this->ChatWindowAction, ETriggerEvent::Triggered, this, &ThisClass::SetFocusOnChatWindow);
	}
}

void ALobbyPlayerController::SetFocusOnChatWindow(const FInputActionValue& Value)
{
	if (this->WasInputKeyJustPressed(EKeys::Enter) && IsValid(this->Lobby))
		this->Lobby->SetFocusOnChatWindow();	
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

void ALobbyPlayerController::Server_SubmitChat_Implementation(const FText& InPlayerName, const FText& InMessage)
{
	for (ALobbyPlayerController* LobbyPlayerController : this->LobbyGameMode->AllPlayerControllers)	
		LobbyPlayerController->Client_UpdateChat(InPlayerName, InMessage);
}

#pragma endregion Server

#pragma region Client

void ALobbyPlayerController::Client_SetupLobbyMenu_Implementation(const FString& ServerName)
{
	if (!ensure(this->LobbyClass != nullptr)) return;

	this->Lobby = CreateWidget<ULobby>(this, this->LobbyClass);
	this->Lobby->InitializeChatWindow();
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

void ALobbyPlayerController::Client_UpdateChat_Implementation(const FText& InPlayerName, const FText& InMessage)
{
	if (IsValid(this->Lobby->ChatWindowClass))
		this->Lobby->UpdateWindowChat(InPlayerName, InMessage);
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
}