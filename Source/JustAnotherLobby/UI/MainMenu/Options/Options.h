// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Common/UserWidgetBase.h"
#include "../../Common/SliderBase.h"
#include "../../../JustAnotherLobbyGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h" 
#include "CommonButtonBase.h"
#include "Options.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UOptions : public UUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	UFUNCTION(BlueprintCallable) void SetTextureQuality(int32 InTextureQuality);
	UFUNCTION(BlueprintCallable) void SetScreenResolution(FIntPoint InScreenResolution);
	UFUNCTION(BlueprintCallable) void SetWindowMode(EWindowMode::Type InWindowMode);
	UFUNCTION(BlueprintCallable) void SetIsOptionMenuInGame(bool InbIsOptionMenuInGame);
	UFUNCTION(BlueprintCallable) bool GetIsOptionMenuInGame();
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> ApplyButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> BackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PreviousTextureQualityButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextTextureQualityButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PreviousResolutionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextResolutionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PreviousWindowModeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextWindowModeButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextureOption;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScreenResolutionOption;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WindowModeOption;

	UFUNCTION() void OnPreviousTextureQualityButtonClicked();
	UFUNCTION() void OnNextTextureQualityButtonClicked();
	UFUNCTION() void OnPreviousResolutionButtonClicked();
	UFUNCTION() void OnNextResolutionButtonClicked();
	UFUNCTION() void OnPreviousWindowModeButtonClicked();
	UFUNCTION() void OnNextWindowModeButtonClicked();

	UFUNCTION() void OnApplyButtonClicked();
	UFUNCTION() void OnBackButtonClicked();

private:
	TMap<int32, FText> TextureQualityMaps;
	TMap<int32, FText> WindowModeMaps;
	TMap<int32, EWindowMode::Type> WindowModeIndexMaps;
	TMap<int32, FText> SupportedResolutionsMaps;
	TArray<FIntPoint> SupportedResolutions;
	bool bIsOptionMenuInGame;
	class UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;

private:
	void SetDefaultGameSettings();
	void InitializeSettings();
	void InitializeScreenResolutionSupported();
	int32 GetTextureIdByValueSelected();
	FIntPoint GetScreenResolutionByValueSelected();
	EWindowMode::Type GetWindowModeByValueSelected();
};