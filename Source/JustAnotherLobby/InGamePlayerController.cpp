// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "InputTriggers.h"
#include "Library/JustAnotherLoobyBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameModes/InGameGameMode.h"

AInGamePlayerController::AInGamePlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    UWorld* World = this->GetWorld();

    if (IsValid(World))
    {
        AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);

        if (IsValid(GameMode))
            InGameGameMode = Cast<AInGameGameMode>(GameMode);

        this->JustAnotherLobbyGameInstance = UJustAnotherLoobyBlueprintLibrary::GetJustAnotherLobbyGameInstance(this);
    }
}

void AInGamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(this->CommonPlayerControllerMappingContext, 0);
    }

    FInputModeGameAndUI FInputModeGameAndUI;

    this->SetInputMode(FInputModeGameAndUI);
}

void AInGamePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
        EnhancedInputComponent->BindAction(this->ShowInGameMenuAction, ETriggerEvent::Triggered, this, &ThisClass::ShowInGameMenu);
    }
}

void AInGamePlayerController::SetCurrentCharacter(ACharacterBase* InCurrentCharacter)
{
    this->CurrentCharacter = InCurrentCharacter;
}

ACharacterBase* AInGamePlayerController::GetCurrentCharacter()
{
    return this->CurrentCharacter;
}

void AInGamePlayerController::ShowInGameMenu(const FInputActionValue& Value)
{
    //if (!ensure(this->InGameMenuClass != nullptr)) return;

    //this->InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
    //this->InGameMenu->Setup();
}

void AInGamePlayerController::Client_ShowLoadingScreen_Implementation()
{
    UJustAnotherLoobyBlueprintLibrary::ShowLoadingScreen(this, true, 3);
}

void AInGamePlayerController::HideInGameMenu()
{
    //this->InGameMenu->TearDown();
}