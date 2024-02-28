// Fill out your copyright notice in the Description page of Project Settings.

#include "OverheadPlayerSpot.h"

void UOverheadPlayerSpot::UpdatePlayerName()
{
    if (IsValid(this->CharacterBase))
        this->LobbyPlayerName->SetText(FText::FromString(this->CharacterBase->PlayerName));
}

void UOverheadPlayerSpot::UpdateReadyStatus()
{
    if (IsValid(this->CharacterBase)) {

        UTexture2D* ReadyStatusIconTexture = this->CharacterBase->bIsReady ?
            Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/UI/Materials/Textures/checkmark-small")))
            :
            Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/UI/Materials/Textures/cancel-small")));

        if (IsValid(ReadyStatusIconTexture))
            this->ReadyStatusIcon->SetBrushFromTexture(ReadyStatusIconTexture);

        FString color = this->CharacterBase->bIsReady ? FString("#38E33F") : FString("#FF3122");

        this->SetPlayerNameColor(color);
    }
}

void UOverheadPlayerSpot::SetCharacter(ACharacterBase* InCharacterBase)
{
    this->CharacterBase = InCharacterBase;
}

void UOverheadPlayerSpot::SetReadyStatusVisibility(bool bIsHidden)
{
    float Opacity = bIsHidden ? 0 : 1;

    this->ReadyStatusIcon->SetRenderOpacity(Opacity);
    this->ReadyStatusIcon->SetOpacity(Opacity);
}

void UOverheadPlayerSpot::SetPlayerNameColor(FString Color)
{
    FColor HexColor = FColor::FromHex(Color);

    FLinearColor LinearHexColor(HexColor);

    this->LobbyPlayerName->SetColorAndOpacity(FSlateColor(LinearHexColor));
}

UImage* UOverheadPlayerSpot::GetStatusIcon()
{
    return this->ReadyStatusIcon;
}

void UOverheadPlayerSpot::NativeDestruct()
{
    RemoveFromParent();

    Super::NativeDestruct();
}