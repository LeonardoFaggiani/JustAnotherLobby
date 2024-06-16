// Fill out your copyright notice in the Description page of Project Settings.


#include "JustAnotherLoobyBlueprintLibrary.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "JustAnotherLobbyLoadingScreen/Public/JustAnotherLobbyLoadingScreenModule.h"


UJustAnotherLoobyBlueprintLibrary::UJustAnotherLoobyBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{ }

void UJustAnotherLoobyBlueprintLibrary::ShowLoadingScreen(const UObject* WorldContextObject, bool bPlayUntilStopped, float PlayTime)
{
    IJustAnotherLobbyLoadingScreenModule& LoadingScreenModule = IJustAnotherLobbyLoadingScreenModule::Get();

    LoadingScreenModule.ShowLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UJustAnotherLoobyBlueprintLibrary::HideLoadingScreen(const UObject* WorldContextObject)
{
    IJustAnotherLobbyLoadingScreenModule& LoadingScreenModule = IJustAnotherLobbyLoadingScreenModule::Get();

    LoadingScreenModule.HideLoadingScreen();
}

UJustAnotherLobbyGameInstance* UJustAnotherLoobyBlueprintLibrary::GetJustAnotherLobbyGameInstance(const UObject* WorldContextObject)
{
    UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance = nullptr;

    if (!WorldContextObject)    
        return nullptr;    

    UWorld* World = WorldContextObject->GetWorld();

    if (!World)    
        return nullptr;
    
    if (IsValid(World->GetGameInstance()))
        JustAnotherLobbyGameInstance = Cast<UJustAnotherLobbyGameInstance>(World->GetGameInstance());

    return JustAnotherLobbyGameInstance;
}

UUserWidget* UJustAnotherLoobyBlueprintLibrary::CreateAndShowWidget(UObject* WorldContextObject, TSubclassOf<UUserWidget> WidgetClass, bool bShowMouseCursor)
{
    if (!WorldContextObject || !WidgetClass)
        return nullptr;    

    UWorld* World = WorldContextObject->GetWorld();

    if (!World)
        return nullptr;    

    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (!PlayerController)
        return nullptr;    

    UUserWidget* Widget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);

    if (!Widget)
        return nullptr;    

    Widget->AddToViewport();

    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(Widget->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = bShowMouseCursor;

    return Widget;
}

bool UJustAnotherLoobyBlueprintLibrary::IsServer(UObject* WorldContextObject)
{
    UWorld* World = WorldContextObject->GetWorld();

    return World->IsServer();
}

ALobbyGameMode* UJustAnotherLoobyBlueprintLibrary::GetLobbyGameMode(const UObject* WorldContextObject)
{
    ALobbyGameMode* LobbyGameMode = nullptr;

    if (!WorldContextObject)
        return nullptr;

    UWorld* World = WorldContextObject->GetWorld();

    if (!World)
        return nullptr;

    AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);

    if(IsValid(GameMode))
        LobbyGameMode = Cast<ALobbyGameMode>(GameMode);

    return LobbyGameMode;
}
