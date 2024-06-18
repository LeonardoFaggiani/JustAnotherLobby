// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWindow.h"

#include "../../JustAnotherLobby/Library/JustAnotherLoobyBlueprintLibrary.h"
#include "JustAnotherLobby/JustAnotherLobbyPlayerState.h"
#include "Kismet/GameplayStatics.h"

bool UChatWindow::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (IsValid(this->ChatInputBox)) {
        this->ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatWindow::PlayOnTextCommitted);
    }

    return true;
}

void UChatWindow::PlayOnTextCommitted(const FText& NewText, ETextCommit::Type TextType)
{
    if (ETextCommit::OnEnter == TextType && !NewText.IsEmpty())
    {
        if (ALobbyPlayerController* LobbyPlayerController = CastChecked<ALobbyPlayerController>(this->GetOwningPlayer())) {
            AJustAnotherLobbyPlayerState* JustAnotherLobbyPlayerState = LobbyPlayerController->GetPlayerState<AJustAnotherLobbyPlayerState>();
            LobbyPlayerController->Server_SubmitChat(FText::FromString(JustAnotherLobbyPlayerState->GetPlayerName()), NewText);
        }
    }
}

void UChatWindow::UpdateChat(const FText& InPlayerName, const FText& InMessage)
{
    UUserWidget* ChatUserWidget = UJustAnotherLoobyBlueprintLibrary::CreateAndShowWidget(this, this->ChatMessage, false, true);

    if (UChatMessage* ChatMessageChecked = CastChecked<UChatMessage>(ChatUserWidget)) {

        ChatMessageChecked->SetPlayerName(InPlayerName);
        ChatMessageChecked->SetMessage(InMessage);

        this->ChatScrollBox->AddChild(ChatMessageChecked);
        this->ChatScrollBox->ScrollToEnd();

        this->ChatInputBox->SetText(FText::FromString(""));
    }
}

void UChatWindow::FocusOnChatInput()
{
    this->ChatInputBox->SetKeyboardFocus();
}
