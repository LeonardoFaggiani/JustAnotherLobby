// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LocationSpot.h"
#include "LocationHeroe.h"
#include "LobbyHeroeSpot.generated.h"

USTRUCT(BlueprintType)
struct FLobbyHeroeSpot
{
    GENERATED_BODY()

public:
    FLobbyHeroeSpot(int32 InIndex, FLocationSpot InLocationSpot, FLocationHeroe InLocationHeroe);
    FLobbyHeroeSpot();    
    ~FLobbyHeroeSpot();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Heroe Spot")
        int32 Index;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Heroe Spot")
        FLocationSpot LocationSpot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Heroe Spot")
        FLocationHeroe LocationHeroe;

};