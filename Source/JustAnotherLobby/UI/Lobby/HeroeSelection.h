// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Common/UserWidgetBase.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "HeroeCard.h"
#include "../../JustAnotherLobbyGameInstance.h"
#include "HeroeSelection.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UHeroeSelection : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHeroeCard> HeroeCard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> BackButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Container;

	void FillContainer();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION() void OnBackButtonClicked();

private:
	class UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
	
};
