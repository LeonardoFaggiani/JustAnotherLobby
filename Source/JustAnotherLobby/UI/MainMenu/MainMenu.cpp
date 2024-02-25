// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../../JustAnotherLobbyGameInstance.h"

bool UMainMenu::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (HostButton && FindGamesButton && OptionsButton && ExitButton) {
        HostButton->OnClicked().AddUObject(this, &ThisClass::OnHostButtonClicked);
        FindGamesButton->OnClicked().AddUObject(this, &ThisClass::OnFindGamesButtonClicked);
        OptionsButton->OnClicked().AddUObject(this, &ThisClass::OnOptionsButtonButtonClicked);
        ExitButton->OnClicked().AddUObject(this, &ThisClass::OnExitButtonClicked);
    }

    UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

        if (IsValid(GameInstance))
            this->JustAnotherLobbyGameInstance = Cast<UJustAnotherLobbyGameInstance>(GameInstance);
    }

    return true;
}

void UMainMenu::NativeDestruct()
{
    //TearDown();

    Super::NativeDestruct();
}

void UMainMenu::OnHostButtonClicked()
{
    this->JustAnotherLobbyGameInstance->SetHostGame(true);
    this->JustAnotherLobbyGameInstance->SetFindGames(false);
    this->JustAnotherLobbyGameInstance->SetOptionsMenu(false);
}

void UMainMenu::OnFindGamesButtonClicked()
{
    this->JustAnotherLobbyGameInstance->SetFindGames(true);
    this->JustAnotherLobbyGameInstance->SetHostGame(false);
    this->JustAnotherLobbyGameInstance->SetOptionsMenu(false);
}

void UMainMenu::OnOptionsButtonButtonClicked()
{
    this->JustAnotherLobbyGameInstance->SetOptionsMenu(true);
    this->JustAnotherLobbyGameInstance->SetHostGame(false);
    this->JustAnotherLobbyGameInstance->SetFindGames(false);
}

void UMainMenu::OnExitButtonClicked() {
    //if (MenuInterface != nullptr) MenuInterface->QuitGame();
}