// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerKickNameIndex.generated.h"

USTRUCT(BlueprintType)
struct FPlayerKickNameIndex
{

    GENERATED_BODY()

public:
    FPlayerKickNameIndex(int32 InIndex, FText InName, bool bIsReady);
    FPlayerKickNameIndex();
    ~FPlayerKickNameIndex();

    UPROPERTY(EditAnywhere)
    int32 Index;

    UPROPERTY(EditAnywhere)
    FText Name;

    UPROPERTY(EditAnywhere)
    bool IsReady;
};