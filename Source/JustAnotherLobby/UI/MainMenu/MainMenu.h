// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonButtonBase.h"
#include "../Common/UserWidgetBase.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UMainMenu : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> HostButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> FindGamesButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> ExitButton;

	UFUNCTION() void OnHostButtonClicked();
	UFUNCTION() void OnFindGamesButtonClicked();
	UFUNCTION() void OnOptionsButtonButtonClicked();
	UFUNCTION() void OnExitButtonClicked();

private:
	class UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
};
