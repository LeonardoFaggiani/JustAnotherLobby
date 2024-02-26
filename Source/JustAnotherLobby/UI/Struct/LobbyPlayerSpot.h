// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LobbyPlayerSpot.generated.h"

USTRUCT(BlueprintType)
struct FLobbyPlayerSpot
{

    GENERATED_BODY()

public:
    FLobbyPlayerSpot();
    FLobbyPlayerSpot(int32 InIndex, FVector InLocation, FRotator InRotation);
    ~FLobbyPlayerSpot();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Spot")
    int32 Index;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Spot")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Player Spot")
    FRotator Rotation;

};