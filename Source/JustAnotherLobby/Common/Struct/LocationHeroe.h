// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LocationHeroe.generated.h"

USTRUCT(BlueprintType)
struct FLocationHeroe
{

    GENERATED_BODY()

public:
    FLocationHeroe(FVector InLocation, FRotator InRotation);
    FLocationHeroe();    
    ~FLocationHeroe();

    UPROPERTY(EditAnywhere)
        FVector Location;

    UPROPERTY(EditAnywhere)
        FRotator Rotation;
};