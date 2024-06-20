// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enum/AttributeType.h"
#include "HeroeAttribute.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FHeroeAttribute
{
    GENERATED_BODY()

public:
    FHeroeAttribute(FString InIcon, int32 InPoints, TEnumAsByte<EAttributeType> InAttributeType);
    FHeroeAttribute();
    ~FHeroeAttribute();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<EAttributeType> AttributeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Points;

};