// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UUserWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	void Setup(int32 zOrder = 0);

	void TearDown();
	
};
