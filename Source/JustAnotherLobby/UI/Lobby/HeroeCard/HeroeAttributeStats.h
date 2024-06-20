// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Common/UserWidgetBase.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "HeroeAttributeStats.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UHeroeAttributeStats : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetIcon(FString InIconImage);

	UFUNCTION(BlueprintCallable)
	void SetPoints(int32 InAttributePoint);

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "Stats")
	UImage* Icon;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "Stats")
	TObjectPtr<UCommonTextBlock> Points;
};
