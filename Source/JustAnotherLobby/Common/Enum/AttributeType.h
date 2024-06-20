// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EAttributeType
{
    Attack UMETA(DisplayName = "Attack"),
    Health UMETA(DisplayName = "Health"),
    Armor UMETA(DisplayName = "Armor")
};