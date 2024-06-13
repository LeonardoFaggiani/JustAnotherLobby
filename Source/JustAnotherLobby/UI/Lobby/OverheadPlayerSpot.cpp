// Fill out your copyright notice in the Description page of Project Settings.

#include "OverheadPlayerSpot.h"

void UOverheadPlayerSpot::UpdatePlayerName(FString InPlayerName)
{
    this->LobbyPlayerName->SetText(FText::FromString(InPlayerName));    
}

void UOverheadPlayerSpot::UpdateReadyStatus(bool bIsReady)
{
    UTexture2D* ReadyStatusIconTexture = bIsReady ?
        Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/UI/Common/Textures/ReadyOk")))
        :
        Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/UI/Common/Textures/UnReady")));

    if (IsValid(ReadyStatusIconTexture))
        this->ReadyStatusIcon->SetBrushFromTexture(ReadyStatusIconTexture);

    FString color = bIsReady ? FString("#6acc69") : FString("#e64849");

    this->SetPlayerNameColor(color);
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