// Fill out your copyright notice in the Description page of Project Settings.


#include "JustAnotherLobbyGameInstance.h"
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>

UJustAnotherLobbyGameInstance::UJustAnotherLobbyGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}



void UJustAnotherLobbyGameInstance::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {

    if (InLevel == nullptr) {
        if (InWorld->GetFName() == FName("MainMenuWidgets"))
        {

        }
    }
}

void UJustAnotherLobbyGameInstance::Init()
{
    Super::Init();

    FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &ThisClass::OnLevelRemovedFromWorld);
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UJustAnotherLobbyGameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UJustAnotherLobbyGameInstance::EndLoadingScreen);

    if (!ensure(this->LoadingScreenClass != nullptr)) return;

    this->LoadingScreen = CreateWidget<ULoadingScreen>(this, LoadingScreenClass);

    //this->LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 5;
    //this->LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    //this->LoadingScreenAttributes.bMoviesAreSkippable = false;
    //this->LoadingScreenAttributes.bWaitForManualStop = true;
    //this->LoadingScreenAttributes.bAllowEngineTick = true;
    //this->LoadingScreenAttributes.WidgetLoadingScreen = this->LoadingScreen->TakeWidget();
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


void UJustAnotherLobbyGameInstance::EndLoadingScreen_Implementation(UWorld* InLoadedWorld)
{
    if (InLoadedWorld->GetName() == "LobbyChampionSelection")
    {
        if (!this->LoadingScreen->IsInViewport()) {
            this->ShowLoadingScreen(false);
            //this->LoadingScreen->SetMenuInterface(this);
        }
    }
}

void UJustAnotherLobbyGameInstance::BeginLoadingScreen_Implementation(const FString& InMapName)
{
    if (InMapName == "/Game/ThirdPerson/Maps/LobbyChampionSelection")
    {
        if (this->LoadingScreen->IsInViewport()) {

            this->LoadingScreen->TransBounceInCompleted();

            //this->LoadingScreen->SetMenuInterface(this);
        }
    }
}

void UJustAnotherLobbyGameInstance::ShowLoadingScreen(bool bWithTransition)
{
    UE_LOG(LogTemp, Warning, TEXT("ShowLoadingScreen OK!"));

    if (!ensure(this->LoadingScreen != nullptr)) return;

    if (!this->LoadingScreen->IsInViewport()) {
        //this->LoadingScreen->Setup(1);
    }

    bWithTransition ? this->LoadingScreen->TransBounceIn() : this->LoadingScreen->TransBounceInCompleted();

    //this->LoadingScreen->SetMenuInterface(this);
}

void UJustAnotherLobbyGameInstance::HideLoadingScreen(bool bWithTransition)
{
    UE_LOG(LogTemp, Warning, TEXT("HideLoadingScreen OK!"));

    if (!ensure(this->LoadingScreen != nullptr)) return;

    if (!this->LoadingScreen->IsInViewport())
        //this->LoadingScreen->Setup();

    this->LoadingScreen->TransBounceOut();
    //this->LoadingScreen->SetMenuInterface(this);
}

void UJustAnotherLobbyGameInstance::OpenNextLevel(FName InLevel, bool bIsListen, bool bShowLoading, float InOpenLevelDelay)
{
    UWorld* World = GetWorld();

    if (World) {

        if (bShowLoading)
            this->ShowLoadingScreen(true);

        FTimerHandle MemberTimerHandle;
        FTimerDelegate TimerDel;

        FString ListenLevel = bIsListen ? "listen" : "";

        TimerDel.BindUFunction(this, FName("OpenLevelWithDelay"), InLevel, ListenLevel);

        GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, TimerDel, InOpenLevelDelay, false);
    }
}


void UJustAnotherLobbyGameInstance::OpenLevelWithDelay(FName InLevelName, FString InListen)
{
    UGameplayStatics::OpenLevel(GWorld, InLevelName, true, InListen);
}