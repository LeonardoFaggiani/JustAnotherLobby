// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerKickName.h"

#include "Net/UnrealNetwork.h"

void UPlayerKickName::SetPlayerName(FText InPlayerName)
{
    this->PlayerNameToKick->SetText(InPlayerName);
}

void UPlayerKickName::SetPlayerId(int32 InId)
{
    this->PlayerId = InId;
}

void UPlayerKickName::SetReadyStatus(bool bIsReady)
{
    FString color = bIsReady ? FString("#6acc69") : FString("#e64849");

    FColor HexColor = FColor::FromHex(color);

    FLinearColor LinearHexColor(HexColor);

    this->PlayerNameToKick->SetColorAndOpacity(FSlateColor(LinearHexColor));
}

int32 UPlayerKickName::GetPlayerId()
{
    return this->PlayerId;
}

void UPlayerKickName::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerKickName, PlayerId);
}