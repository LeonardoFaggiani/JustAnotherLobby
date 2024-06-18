// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Common/UserWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "ChatMessage.h"

#include "ChatWindow.generated.h"

/**
 * 
 */
UCLASS()
class JUSTANOTHERLOBBY_API UChatWindow : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatInputBox;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatScrollBox;

	void UpdateChat(const FText& InPlayerName, const FText& InMessage);
	void FocusOnChatInput();

	UPROPERTY(EditAnyWhere)
	TSubclassOf<UChatMessage> ChatMessage;

protected:

	virtual bool Initialize() override;

	UFUNCTION() void PlayOnTextCommitted(const FText& NewText, ETextCommit::Type TextType);
};
