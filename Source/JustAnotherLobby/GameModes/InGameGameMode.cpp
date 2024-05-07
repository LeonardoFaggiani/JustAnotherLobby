// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGameMode.h"
#include "Net/UnrealNetwork.h"
#include "../JustAnotherLobbyPlayerState.h"
#include "Kismet/GameplayStatics.h"


AInGameGameMode::AInGameGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		UGameInstance* GameInstance = World->GetGameInstance();

		if (IsValid(GameInstance)) {
			this->JustAnotherLobbyGameInstance = Cast<UJustAnotherLobbyGameInstance>(GameInstance);
		}
	}
}

void AInGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		if (NewPlayer) {
			AInGamePlayerController* InGamePlayerController = Cast<AInGamePlayerController>(NewPlayer);

			this->AllPlayerControllers.Add(InGamePlayerController);

			if (IsValid(this->JustAnotherLobbyGameInstance))
				this->Server_RespawnPlayer(InGamePlayerController);
		}
	}
}

void AInGameGameMode::Logout(AController* Exiting)
{

}

void AInGameGameMode::Server_RespawnPlayer_Implementation(AInGamePlayerController* InGamePlayerController)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	int32 InPlayerIndex = this->AllPlayerControllers.IndexOfByKey(InGamePlayerController);

	FTransform Transform = this->GetTransformOfPlayerStartByIndex(InPlayerIndex);

	FInGamePlayerInfo* InGamePlayerInfo = this->GetPlayerInfoByIndex((InPlayerIndex + 1));

	ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetWorld()->SpawnActor<ACharacterBase>(InGamePlayerInfo->HeroeSelected, Transform, params));

	FTimerHandle MemberTimerHandle;
	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("Server_SetOverheadPlayer"), true, FString("FFFFFF"));

	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, TimerDel, 0.1f, false);

	InGamePlayerController->SetCurrentCharacter(CharacterBase);
	InGamePlayerController->Possess(CharacterBase);
}

void AInGameGameMode::Server_UpdatePlayerName_Implementation()
{
	for (AInGamePlayerController* InGamePlayerController : this->AllPlayerControllers)
		this->UpdatePlayerName(InGamePlayerController);
}

void AInGameGameMode::Server_SetOverheadPlayer_Implementation(bool bIsHiddenIcon, const FString& ColorName)
{
	for (AInGamePlayerController* InGamePlayerController : this->AllPlayerControllers) {

		ACharacterBase* CharacterBase = InGamePlayerController->GetCurrentCharacter();

		if (IsValid(CharacterBase))
			CharacterBase->Multi_SetIconAndColorOverheadWidget(bIsHiddenIcon, ColorName);
	}

}

void AInGameGameMode::UpdatePlayerName(AInGamePlayerController* InGamePlayerController)
{
	AJustAnotherLobbyPlayerState* JustAnotherLobbyPlayerState = InGamePlayerController->GetPlayerState<AJustAnotherLobbyPlayerState>();

	ACharacterBase* CharacterBase = InGamePlayerController->GetCurrentCharacter();

	if (IsValid(CharacterBase) && IsValid(JustAnotherLobbyPlayerState))
		CharacterBase->Multi_SetPlayerName(JustAnotherLobbyPlayerState->GetPlayerName());
}


TArray<AActor*> AInGameGameMode::GetAllPlayersStart()
{
	TArray<AActor*> AllPlayersStart;

	this->PlayerStart = APlayerStart::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), this->PlayerStart, AllPlayersStart);

	return AllPlayersStart;
}

FTransform AInGameGameMode::GetTransformOfPlayerStartByIndex(int32 InPlayerIndex)
{
	TArray<AActor*> PlayersStart = this->GetAllPlayersStart();

	FTransform Transform = PlayersStart[InPlayerIndex]->GetTransform();

	return Transform;
}

FInGamePlayerInfo* AInGameGameMode::GetPlayerInfoByIndex(int32 InPlayerIndex)
{
	auto GetPlayerByIndex = [InPlayerIndex](const FInGamePlayerInfo& Player) {
		return Player.PlayerIndex == InPlayerIndex;
		};

	FInGamePlayerInfo* InGamePlayerInfo = this->JustAnotherLobbyGameInstance->InGamePlayersInfo.FindByPredicate(GetPlayerByIndex);

	return InGamePlayerInfo;
}

void AInGameGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameGameMode, ConnectedPlayers);
	DOREPLIFETIME(AInGameGameMode, AllPlayerControllers);
}
