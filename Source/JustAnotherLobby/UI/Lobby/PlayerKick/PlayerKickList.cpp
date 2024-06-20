// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerKickList.h"

#include "../../../PlayerController/LobbyPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/VerticalBoxSlot.h"

void UPlayerKickList::FillPlayerKickContainer(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex)
{
    this->Container->ClearChildren();

    if (InPlayerNamesIndex.Num() > 0)
    {
        for (FPlayerKickNameIndex Player : InPlayerNamesIndex) {

            UPlayerKickName* InPlayerKickName = CreateWidget<UPlayerKickName>(this, this->PlayerKickName);

            InPlayerKickName->SetPlayerName(Player.Name);
            InPlayerKickName->SetPlayerId(Player.Index);
            InPlayerKickName->SetReadyStatus(Player.IsReady);

            UVerticalBoxSlot* VerticalBoxSlot = this->Container->AddChildToVerticalBox(InPlayerKickName);

            VerticalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);
        }
    }
}

void UPlayerKickList::SetReadyStatus(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex)
{
    TArray<UUserWidget*> FoundWidgets = TArray<UUserWidget*>();

    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UPlayerKickName::StaticClass(), false);

    for (UUserWidget* PlayerKickNameWidget : FoundWidgets)
    {
        UPlayerKickName* PlayerKickNameFounded = Cast<UPlayerKickName>(PlayerKickNameWidget);

        if (InPlayerNamesIndex.IsValidIndex(PlayerKickNameFounded->GetPlayerId())) {

            FPlayerKickNameIndex PlayerKickNameIndex = InPlayerNamesIndex[PlayerKickNameFounded->GetPlayerId()];

            PlayerKickNameFounded->SetReadyStatus(PlayerKickNameIndex.IsReady);
        }
    }
}