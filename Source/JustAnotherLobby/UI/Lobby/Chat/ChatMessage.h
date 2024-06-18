// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Common/UserWidgetBase.h"
#include "Components/TextBlock.h"

#include "ChatMessage.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UChatMessage : public UUserWidgetBase
{
	GENERATED_BODY()

public:	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageText;

	UFUNCTION(BlueprintCallable) void SetPlayerName(FText InPlayerName);
	UFUNCTION(BlueprintCallable) void SetMessage(FText InMessage);


private:
	FText PlayerName;
	FText Message;
};
