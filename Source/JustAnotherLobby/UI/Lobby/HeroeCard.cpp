// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeCard.h"
#include "../../LobbyPlayerController.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../../Library/JustAnotherLoobyBlueprintLibrary.h"
#include "../../JustAnotherLobbyGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UHeroeCard::UHeroeCard()
{
    this->OnClicked().AddUObject(this, &UHeroeCard::SetHeroeSelected);
}


void UHeroeCard::SetHeroeSelected()
{
    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    if (IsValid(LobbyPlayerController)) {

        RemoveDisabledStateToAllItems();

        UWorld* World = GetWorld();

        if (IsValid(World))
        {
            this->SetIsEnabled(false);
            
            UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance = UJustAnotherLoobyBlueprintLibrary::GetJustAnotherLobbyGameInstance(this);
            
            TSubclassOf<ACharacterBase> CharacterBase = JustAnotherLobbyGameInstance->GetHeroeByName(this->HeroeName->GetText().ToString());
            
            LobbyPlayerController->Client_AssignHeroeToPlayer(CharacterBase);            
        }
    }
}

void UHeroeCard::SetAttributeStats(const FHeroes& InHeroes)
{
    //For each heroe set the status
    for (const FHeroeAttribute& HeroeAttribute : InHeroes.Attributes)
    {
        UHeroeAttributeStats* InHeroeAttributeStats = CreateWidget<UHeroeAttributeStats>(this, this->HeroeAttributeStats);

        InHeroeAttributeStats->SetIcon(HeroeAttribute.Icon);
        InHeroeAttributeStats->SetPoints(HeroeAttribute.Points);

        UHorizontalBoxSlot* HorizontalBoxSlot = this->StatsContainer->AddChildToHorizontalBox(InHeroeAttributeStats);

        FSlateChildSize SlateChildSize;

        HorizontalBoxSlot->SetSize(SlateChildSize);
    }
}

void UHeroeCard::SetHeroeName(FString InHeroeName)
{
    this->HeroeName->SetText(FText::FromString(InHeroeName));
}

void UHeroeCard::SetHeroeIcon(FString InHeroeIcon)
{
    UTexture2D* IconTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *InHeroeIcon));

    if (IsValid(IconTexture))
        this->HeroeIcon->SetBrushFromTexture(IconTexture);
}

void UHeroeCard::RemoveDisabledStateToAllItems()
{
    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    TArray<UUserWidget*> FoundWidgets = TArray<UUserWidget*>();

    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UHeroeSelection::StaticClass());

    for (UUserWidget* HeroeSelectionWidget : FoundWidgets)
    {
        UHeroeSelection* HeroeSelection = Cast<UHeroeSelection>(HeroeSelectionWidget);
        HeroeSelection->GetHeroeCards();

        for (UHeroeCard* HeroeCard : HeroeSelection->GetHeroeCards())
            HeroeCard->SetIsEnabled(true);
    }
}