// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../UI/Lobby/HeroeCard/HeroeCard.h"
#include "JustAnotherLobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API AJustAnotherLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

	AJustAnotherLobbyPlayerState();

private:
	TArray<UHeroeCard*> HeroesCards;
	
};
