// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "CommonButtonBase.h"
#include "../Common/UserWidgetBase.h"
#include "HeroeSelection.h"
#include "../../JustAnotherLobbyGameInstance.h"
#include "../Struct/ConfigurationMaps.h"
#include "../Struct/LobbyPlayerInfo.h"

#include "Lobby.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API ULobby : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UOverlay* CharacterSelectionContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> ReadyButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> ReadyUpButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> HeroesButton;

	void SetCurrentPlayersFormat(FString InCurrentPlayersFormat);
	void SetServerName(FString serverName);
	void UpdateStatus();
	void SetMap(UTexture2D* mapImage, FString mapName);
	void ShowOrHideButton();
	void SetEnablePlayButton(bool bEnabled);
	void SetHiddenHeroesButton(bool bHidden);

protected:

	virtual bool Initialize() override;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<UHeroeSelection> HeroeSelection;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentPlayersFormat;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(Replicated, meta = (BindWidget))
	class UImage* MapImage;

	UPROPERTY(Replicated, meta = (BindWidget))
	class UTextBlock* MapName;

	UPROPERTY(meta = (BindWidget))
	class UButton* PreviousMap;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextMap;

	UFUNCTION() void OnReadyButtonClicked();
	UFUNCTION() void OnPlayButtonClicked();
	UFUNCTION() void OnPreviousMapButtonClicked();
	UFUNCTION() void OnNextMapButtonClicked();
	UFUNCTION() void OnHeroesButtonClicked();

private:
	void InitializeMap();
	FConfigurationMaps* GetCurrentMapByName(FString Name);
	FConfigurationMaps GetFirstOrLastMap(bool bIsFirst);
	FConfigurationMaps* GetPreviousNextMap(bool IsIncrement);
	void SetMapSelector(FConfigurationMaps* ConfigurationMaps);

	void NotifyMapChaged();
	UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
	class ALobbyGameMode* LobbyGameMode;
	UTexture2D* CurrentTextureMap;

};
