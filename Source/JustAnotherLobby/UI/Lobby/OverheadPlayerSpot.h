// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Common/UserWidgetBase.h"
#include "OverheadPlayerSpot.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UOverheadPlayerSpot : public UUserWidgetBase
{
	GENERATED_BODY()

public:

	void UpdateReadyStatus(bool bIsReady);
	void UpdatePlayerName(FString InPlayerName);

	void SetReadyStatusVisibility(bool bIsHidden);
	void SetPlayerNameColor(FString InColor);

	UFUNCTION(BlueprintCallable)
		UImage* GetStatusIcon();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* LobbyPlayerName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* ReadyStatusIcon;

protected:
	virtual void NativeDestruct() override;
};
