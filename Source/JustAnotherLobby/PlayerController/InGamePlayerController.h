// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Characters/CharacterBase.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInGamePlayerController(const FObjectInitializer& ObjectInitializer);

	//UPROPERTY(EditAnyWhere)
	//TSubclassOf<UInGameMenu> InGameMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* CommonPlayerControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShowInGameMenuAction;

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ShowLoadingScreen();
	void Client_ShowLoadingScreen_Implementation();

	void SetCurrentCharacter(ACharacterBase* InCurrentCharacter);
	ACharacterBase* GetCurrentCharacter();

	void HideInGameMenu();

private:
	void ShowInGameMenu(const FInputActionValue& Value);
	class AInGameGameMode* InGameGameMode;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	class UInGameMenu* InGameMenu;
	ACharacterBase* CurrentCharacter;
	class UJustAnotherLobbyGameInstance* JustAnotherLobbyGameInstance;
	
};
