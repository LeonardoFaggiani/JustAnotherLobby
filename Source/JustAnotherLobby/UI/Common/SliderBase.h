// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "Components/Slider.h"
#include "CommonTextBlock.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"
#include "SliderBase.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API USliderBase : public UUserWidgetBase
{
	GENERATED_BODY()

	virtual bool Initialize();

public:
	UPROPERTY(EditAnywhere, Category = "Sound Settings")
	FString Title;

	UPROPERTY(EditAnywhere, Category = "Sound Settings")
	USoundClass* SoundClass;

	UPROPERTY(EditAnywhere, Category = "Sound Settings")
	USoundMix* SoundMix;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* Description;
	
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeRange;

	UFUNCTION() void OnVolumeValueChanged(float Value);
	UFUNCTION(BlueprintCallable) void SetTitle(FText InTitle);
	UFUNCTION(BlueprintCallable) void GetTitle();
};
