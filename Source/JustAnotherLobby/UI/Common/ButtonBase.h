// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "ButtonBase.generated.h"

/**
 *
 */
UCLASS()
class JUSTANOTHERLOBBY_API UButtonBase : public UCommonButtonBase
{
    GENERATED_BODY()

    virtual bool Initialize();

    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* ButtonTitle;

    UPROPERTY(EditAnywhere, Category = "Settings")
    FString ButtonText;

public:
    UFUNCTION(BlueprintCallable) void SetText(FText InText);
};
