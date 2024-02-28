// Fill out your copyright notice in the Description page of Project Settings.


#include "JustAnotherLobbyPlayerState.h"

AJustAnotherLobbyPlayerState::AJustAnotherLobbyPlayerState() {

}

void AJustAnotherLobbyPlayerState::AddHeroeCard(UHeroeCard* InHeroeCard)
{
    this->HeroesCards.Add(InHeroeCard);
}

TArray<UHeroeCard*> AJustAnotherLobbyPlayerState::GetHeroesCards()
{
    return this->HeroesCards;
}