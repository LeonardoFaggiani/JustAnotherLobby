// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Animation/WidgetAnimation.h"

#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API ULoadingScreen : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void TransBounceIn();
	UFUNCTION(BlueprintCallable)
	void TransBounceOut();
	UFUNCTION(BlueprintCallable)
	void TransBounceInCompleted();

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BounceInOut;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BounceInCompleted;
	
};
