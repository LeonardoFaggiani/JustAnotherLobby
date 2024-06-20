// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Common/UserWidgetBase.h"
#include "Components/TextBlock.h"
#include "PlayerKickName.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UPlayerKickName : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameToKick;	

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerId();

	void SetPlayerName(FText InPlayerName);
	void SetPlayerId(int32 InId);
	void SetReadyStatus(bool bIsReady);	

	UPROPERTY(Replicated)
	int32 PlayerId;
};
