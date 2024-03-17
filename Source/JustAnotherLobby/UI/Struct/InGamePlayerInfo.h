// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Characters/CharacterBase.h"
#include "InGamePlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FInGamePlayerInfo
{
    GENERATED_BODY()

public:
    FInGamePlayerInfo();
    ~FInGamePlayerInfo();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InGame Player Info")
    FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InGame Player Info")
    TSubclassOf<ACharacterBase> HeroeSelected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InGame Player Info")
    int32 PlayerIndex;
};