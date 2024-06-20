// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConfigurationMaps.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FConfigurationMaps
{
	GENERATED_BODY()

public:
	FConfigurationMaps();
	FConfigurationMaps(FString InPath, FString InImagePath, FString InName, int32 InOrder);
	~FConfigurationMaps();

	UPROPERTY(EditAnyWhere)
	FString Path;
	UPROPERTY(EditAnyWhere)
	FString ImagePath;
	UPROPERTY(EditAnyWhere)
	FString Name;
	UPROPERTY(EditAnyWhere)
	int32 Order;


};
