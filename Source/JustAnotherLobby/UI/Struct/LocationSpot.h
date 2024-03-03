// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LocationSpot.generated.h"

USTRUCT(BlueprintType)
struct FLocationSpot
{

    GENERATED_BODY()

public:
    FLocationSpot(FVector InLocation, FRotator InRotation);
    FLocationSpot();
    ~FLocationSpot();

    UPROPERTY(EditAnywhere)
        FVector Location;

    UPROPERTY(EditAnywhere)
        FRotator Rotation;
};