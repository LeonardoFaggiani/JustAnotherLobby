// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatMessage.h"

void UChatMessage::SetPlayerName(FText InPlayerName)
{
    this->PlayerName = InPlayerName;
    this->PlayerNameText->SetText(this->PlayerName);
}

void UChatMessage::SetMessage(FText InMessage)
{
    this->Message = InMessage;
    this->MessageText->SetText(this->Message);
}