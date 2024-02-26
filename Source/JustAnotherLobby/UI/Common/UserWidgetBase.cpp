// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetBase.h"

void UUserWidgetBase::Setup(int32 zOrder)
{
	AddToViewport(zOrder);

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UUserWidgetBase::TearDown()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (PlayerController == nullptr) return;

	FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}