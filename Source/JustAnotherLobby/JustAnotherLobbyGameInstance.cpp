// Fill out your copyright notice in the Description page of Project Settings.


#include "JustAnotherLobbyGameInstance.h"
#include "TimerManager.h"
#include "Runtime/UMG/Public/UMG.h"
#include "JustAnotherLobbyLoadingScreen.h"
#include "Net/UnrealNetwork.h"
#include "MoviePlayer.h"
#include <Kismet/GameplayStatics.h>

UJustAnotherLobbyGameInstance::UJustAnotherLobbyGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

UMainMenu* UJustAnotherLobbyGameInstance::LoadMenu()
{
    if (!ensure(MenuClass != nullptr)) return nullptr;
    this->Menu = CreateWidget<UMainMenu>(this, this->MenuClass);

    if (!ensure(Menu != nullptr)) return nullptr;

    this->Menu->Setup();

    return this->Menu;
}

void UJustAnotherLobbyGameInstance::SetHostGame(bool InbIsHostGameMenu)
{
    this->bIsHostGameMenu = InbIsHostGameMenu;
}

void UJustAnotherLobbyGameInstance::SetFindGames(bool InbIsFindGamesMenu)
{
    this->bIsFindGamesMenu = InbIsFindGamesMenu;
}

void UJustAnotherLobbyGameInstance::SetOptionsMenu(bool InbIsOptionsMenu)
{
    this->bIsOptionsMenu = InbIsOptionsMenu;
}

void UJustAnotherLobbyGameInstance::SetBackToMainMenu(bool InbIsBackToMainMenu)
{
    this->bIsBackToMainMenu = InbIsBackToMainMenu;
}

bool UJustAnotherLobbyGameInstance::GetHostGame()
{
    return this->bIsHostGameMenu;
}

bool UJustAnotherLobbyGameInstance::GetFindGames()
{
    return this->bIsFindGamesMenu;
}

bool UJustAnotherLobbyGameInstance::GetOptionsMenu()
{
    return this->bIsOptionsMenu;
}

bool UJustAnotherLobbyGameInstance::GetBackToMainMenu()
{
    return this->bIsBackToMainMenu;
}

void UJustAnotherLobbyGameInstance::SetHostSettings(int32 InNumberOfPlayers, FString InServerName) {
    this->MaxPlayers = InNumberOfPlayers;
    this->ServerName = InServerName;
}

void UJustAnotherLobbyGameInstance::PlayEnviromentMusic(USoundBase* Audio, float Volume, bool bIsPersistLevel)
{
    if (!IsValid(Audio)) return;

    this->Music = UGameplayStatics::CreateSound2D(GetWorld(), Audio, Volume, 1, 0, nullptr, bIsPersistLevel, true);

    this->Music->Play();
}

void UJustAnotherLobbyGameInstance::StopEnviromentMusic()
{
    if (!IsValid(this->Music)) return;

    this->Music->Stop();
}

bool UJustAnotherLobbyGameInstance::GetIsFirstTimeLoading()
{
    return this->bIsFirstTimeLoading;
}

void UJustAnotherLobbyGameInstance::SetFirstTimeLoading(bool InbIsFirstTimeLoading)
{
    this->bIsFirstTimeLoading = InbIsFirstTimeLoading;
}

TSubclassOf<ACharacterBase> UJustAnotherLobbyGameInstance::GetHeroeByName(FString InHeroeName)
{
    auto GetHeroeByName = [InHeroeName](const FHeroes& Heroe) {
        return Heroe.Name == InHeroeName;
        };

    auto InHeroeResource = this->Heroes.FindByPredicate(GetHeroeByName);

    return InHeroeResource->TargetClass;
}

void UJustAnotherLobbyGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UJustAnotherLobbyGameInstance, MaxPlayers);
    DOREPLIFETIME(UJustAnotherLobbyGameInstance, ServerName);
}