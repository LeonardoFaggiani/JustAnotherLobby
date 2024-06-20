// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Common/UserWidgetBase.h"
#include "Components/VerticalBox.h"
#include "../../../Common/Struct/PlayerKickNameIndex.h"
#include "PlayerKickName.h"
#include "PlayerKickList.generated.h"

class ALobbyPlayerController;

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UPlayerKickList : public UUserWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnyWhere)
	TSubclassOf<UPlayerKickName> PlayerKickName;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* Container;

	void FillPlayerKickContainer(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex);
	void SetReadyStatus(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex);
};
