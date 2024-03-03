// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/WidgetComponent.h"
#include "../UI/Lobby/OverheadPlayerSpot.h"
#include "CharacterBase.generated.h"

UCLASS()
class JUSTANOTHERLOBBY_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(Replicated)
		bool bIsReady;

	UPROPERTY(Replicated)
		FString PlayerName;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* OverheadPlayerSpot;

	UFUNCTION(NetMulticast, Reliable)
		void Multi_SetPlayerName(const FString& InPlayerName);
		void Multi_SetPlayerName_Implementation(const FString& InPlayerName);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void Multi_SetReadyStatus(bool InbIsReady);
		void Multi_SetReadyStatus_Implementation(bool InbIsReady);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void Multi_SetIconAndColorOverheadWidget(bool bIsHidden, const FString& InPlayerNameColor);
		void Multi_SetIconAndColorOverheadWidget_Implementation(bool bIsHidden, const FString& InPlayerNameColor);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

private:
	UOverheadPlayerSpot* CharacterOverhead;
};