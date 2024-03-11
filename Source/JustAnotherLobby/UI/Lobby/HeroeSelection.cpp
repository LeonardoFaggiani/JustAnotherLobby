// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeSelection.h"
#include "Components/HorizontalBoxSlot.h"
#include "../../LobbyPlayerController.h"

bool UHeroeSelection::Initialize() {

    if (!Super::Initialize())
        return false;

    UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

        if (IsValid(GameInstance)) {
            JustAnotherLobbyGameInstance = Cast<UJustAnotherLobbyGameInstance>(GameInstance);
        }
    }

    if (BackButton)
        BackButton->OnClicked().AddUObject(this, &UHeroeSelection::OnBackButtonClicked);

    this->FillContainer();

    return true;
}

void UHeroeSelection::OnBackButtonClicked()
{
    RemoveFromParent();

    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    LobbyPlayerController->Client_SwitchToLobbyMode();
}

void UHeroeSelection::FillContainer()
{
    //Fill the container of heroes with the card of each heroes and thier stats
    this->Container->ClearChildren();

    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    if (IsValid(this->JustAnotherLobbyGameInstance))
    {
        for (FHeroes Heroes : this->JustAnotherLobbyGameInstance->Heroes)
        {
            UHeroeCard* InHeroeItem = CreateWidget<UHeroeCard>(this, this->HeroeCard);

            InHeroeItem->SetPadding(FMargin{ 8,0,8,5 });
            InHeroeItem->SetHeroeName(Heroes.Name);
            InHeroeItem->SetHeroeIcon(Heroes.Icon);
            InHeroeItem->SetAttributeStats(Heroes);

            UHorizontalBoxSlot* HorizontalBoxSlot = this->Container->AddChildToHorizontalBox(InHeroeItem);

            HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);

            this->SetHeroeCard(InHeroeItem);
        }
    }
}

TArray<UHeroeCard*> UHeroeSelection::GetHeroeCards()
{
    return this->HeroeCards;
}

void UHeroeSelection::SetHeroeCard(UHeroeCard* InHeroeCard)
{
    this->HeroeCards.Add(InHeroeCard);
}
