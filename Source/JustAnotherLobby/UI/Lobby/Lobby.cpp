// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby.h"

#include "Net/UnrealNetwork.h"
#include "../../GameModes/LobbyGameMode.h"
#include "../../PlayerController/LobbyPlayerController.h"
#include "../../Common/Library/JustAnotherLoobyBlueprintLibrary.h"
#include "Chat/ChatWindow.h"
#include "Kismet/GameplayStatics.h"

bool ULobby::Initialize()
{
    if (!Super::Initialize())
        return false;

    this->JustAnotherLobbyGameInstance = UJustAnotherLoobyBlueprintLibrary::GetJustAnotherLobbyGameInstance(this);
    this->LobbyGameMode = UJustAnotherLoobyBlueprintLibrary::GetLobbyGameMode(this);
    
    if (IsValid(this->LobbyGameMode))    
        this->InitializeMap();    

    if (this->ReadyUpButton && this->ReadyButton && this->PreviousMap && this->NextMap && this->HeroesButton && this->PlayerCountButton) {

        this->ReadyUpButton->OnClicked().AddUObject(this, &ThisClass::OnPlayButtonClicked);
        this->ReadyButton->OnClicked().AddUObject(this, &ThisClass::OnReadyButtonClicked);
        this->HeroesButton->OnClicked().AddUObject(this, &ThisClass::OnHeroesButtonClicked);
        this->PlayerCountButton->OnClicked().AddUObject(this, &ThisClass::OnPlayerCountButtonClicked);

        this->PreviousMap->OnClicked.AddDynamic(this, &ULobby::OnPreviousMapButtonClicked);
        this->NextMap->OnClicked.AddDynamic(this, &ULobby::OnNextMapButtonClicked);

        this->ShowOrHideButton();
    }

    return true;
}

void ULobby::SetServerName(FString serverName) {
    this->ServerName->SetText(FText::FromString(serverName));
}

void ULobby::ShowOrHideButton() {

    if (this->ReadyButton && this->PreviousMap && this->NextMap) {

        if (!UJustAnotherLoobyBlueprintLibrary::IsServer(this)) {
            this->ReadyUpButton->SetVisibility(ESlateVisibility::Hidden);
            this->ReadyButton->SetVisibility(ESlateVisibility::Visible);

            this->PreviousMap->SetVisibility(ESlateVisibility::Collapsed);
            this->NextMap->SetVisibility(ESlateVisibility::Collapsed);
        }

        this->SetEnablePlayButton(false);
    }
}

#pragma region

void ULobby::OnPlayButtonClicked()
{
    if (this->LobbyGameMode->IsAllPlayerReady())
    {
        for (ALobbyPlayerController* PlayerController : this->LobbyGameMode->AllPlayerControllers)
            PlayerController->Client_ShowLoadingScreen();

        this->LobbyGameMode->Server_LaunchTheGame();
    }
}

void ULobby::OnHeroesButtonClicked()
{
    UHeroeSelection* InHeroeSelection = CreateWidget<UHeroeSelection>(this, this->HeroeSelection);

    InHeroeSelection->Setup();

    this->ReadyButton->SetIsEnabled(false);

    this->SetHiddenHeroesButton(true);
}

void ULobby::OnReadyButtonClicked()
{
    UpdateStatus();
}

void ULobby::OnPlayerCountButtonClicked()
{
    if (!IsValid(this->PlayerKickList))
        return;

    if (this->PlayerKickList->GetVisibility() == ESlateVisibility::Visible) {
        this->PlayerKickList->SetVisibility(ESlateVisibility::Hidden);
    }
    else {

        ALobbyPlayerController* CurrentPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

        CurrentPlayerController->Server_FillContainerPlayerKickList();

        this->PlayerKickList->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobby::UpdateStatus()
{
    ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(this->GetOwningPlayer());

    if (IsValid(LobbyPlayerController))
    {
        LobbyPlayerController->PlayerSettings.bPlayerReadyState = !LobbyPlayerController->PlayerSettings.bPlayerReadyState;

        LobbyPlayerController->Server_NotifyPlayerStatus(LobbyPlayerController->PlayerSettings);

        this->HeroesButton->SetIsEnabled(!LobbyPlayerController->PlayerSettings.bPlayerReadyState);
        
        if (IsValid(this->LobbyGameMode))
            this->ReadyUpButton->SetIsEnabled(this->LobbyGameMode->IsAllPlayerReady());        
    }
}

void ULobby::SetHiddenHeroesButton(bool bHidden) {
    this->HeroesButton->SetVisibility(bHidden ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void ULobby::SetEnablePlayButton(bool bEnabled) {
    this->ReadyUpButton->SetIsEnabled(bEnabled);
}

void ULobby::OnPreviousMapButtonClicked()
{
    FConfigurationMaps* ConfigurationMaps = this->GetPreviousNextMap(false);

    this->SetMapSelector(ConfigurationMaps);
}

void ULobby::OnNextMapButtonClicked()
{
    FConfigurationMaps* ConfigurationMaps = this->GetPreviousNextMap(true);

    this->SetMapSelector(ConfigurationMaps);
}

#pragma endregion Buttons

#pragma region

void ULobby::SetFocusOnChatWindow()
{
    if (IsValid(this->ChatWindow))
        this->ChatWindow->FocusOnChatInput();
}

void ULobby::InitializeChatWindow()
{
    UUserWidget* ChatWindowUserWidget = UJustAnotherLoobyBlueprintLibrary::CreateAndShowWidget(this, this->ChatWindowClass, false, true);

    if (UChatWindow* ChatWindowChecked = CastChecked<UChatWindow>(ChatWindowUserWidget)) {

        this->ChatWindow = ChatWindowChecked;

        UOverlaySlot* OverlaySlot = this->ChatWindowBox->AddChildToOverlay(this->ChatWindow);

        OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
        OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
    }
}

void ULobby::UpdateWindowChat(const FText& InPlayerName, const FText& InMessage)
{
    if (IsValid(this->ChatWindow))    
        this->ChatWindow->UpdateChat(InPlayerName, InMessage);    
}

#pragma endregion ChatWindow

#pragma region

void ULobby::InitializeMap()
{
    FConfigurationMaps FConfigurationMaps = GetFirstOrLastMap(true);

    UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FConfigurationMaps.ImagePath));

    this->SetMap(Texture, FConfigurationMaps.Name);
    this->PreviousMap->SetIsEnabled(false);
    
    this->NotifyMapChaged();
}

FConfigurationMaps* ULobby::GetCurrentMapByName(FString Name)
{
    if (this->JustAnotherLobbyGameInstance != nullptr)
    {
        FConfigurationMaps* DefaultMap = this->JustAnotherLobbyGameInstance->ConfigurationMaps.Find(Name);

        return DefaultMap;
    }

    return nullptr;
}

FConfigurationMaps ULobby::GetFirstOrLastMap(bool bIsFirst)
{
    TArray<FConfigurationMaps> ConfigurationDataArray;

    this->JustAnotherLobbyGameInstance->ConfigurationMaps.GenerateValueArray(ConfigurationDataArray);

    ConfigurationDataArray.Sort([bIsFirst](const FConfigurationMaps A, const FConfigurationMaps B) {
        return bIsFirst ? A.Order < B.Order : A.Order > B.Order;
        });

    return ConfigurationDataArray[0];
}

FConfigurationMaps* ULobby::GetPreviousNextMap(bool IsIncrement)
{
    this->PreviousMap->SetIsEnabled(true);
    this->NextMap->SetIsEnabled(true);

    FName CurrentMapName = this->MapImage->Brush.GetResourceName();

    FConfigurationMaps* CurrentMap = this->GetCurrentMapByName(CurrentMapName.ToString());

    if (CurrentMap == nullptr)
        return nullptr;

    FConfigurationMaps* PreviousOrNextMap = nullptr;

    int PreviousNextOrder = IsIncrement ? (CurrentMap->Order + 1) : (CurrentMap->Order - 1);

    for (TPair<FString, FConfigurationMaps> Map : this->JustAnotherLobbyGameInstance->ConfigurationMaps)
    {
        if (Map.Value.Order == PreviousNextOrder)
        {
            PreviousOrNextMap = this->GetCurrentMapByName(Map.Key);
        }
    }

    int VerifyIfExistsPreviousOrNextMap = IsIncrement ? (PreviousNextOrder + 1) : (PreviousNextOrder - 1);

    FConfigurationMaps* ExistsPreviousOrNextMap = nullptr;

    for (TPair<FString, FConfigurationMaps> Map : this->JustAnotherLobbyGameInstance->ConfigurationMaps)
    {
        if (Map.Value.Order == VerifyIfExistsPreviousOrNextMap)
        {
            ExistsPreviousOrNextMap = this->GetCurrentMapByName(Map.Key);
        }
    }

    if (ExistsPreviousOrNextMap == nullptr) {
        if (IsIncrement) {
            this->PreviousMap->SetIsEnabled(true);
            this->NextMap->SetIsEnabled(false);
        }
        else {
            this->PreviousMap->SetIsEnabled(false);
            this->NextMap->SetIsEnabled(true);
        }
    }

    return PreviousOrNextMap;
}

void ULobby::SetMapSelector(FConfigurationMaps* ConfigurationMaps)
{
    if (ConfigurationMaps != nullptr)
    {
        UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ConfigurationMaps->ImagePath));

        this->SetMap(Texture, *ConfigurationMaps->Name);

        this->NotifyMapChaged();
    }
}

void ULobby::FillContainerPlayerKickList(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex)
{
    this->PlayerKickList->FillPlayerKickContainer(InPlayerNamesIndex);
}

void ULobby::UpdateReadyStatusInPLayerKickList(const TArray<FPlayerKickNameIndex>& InPlayerNamesIndex)
{
    this->PlayerKickList->SetReadyStatus(InPlayerNamesIndex);
}

void ULobby::NotifyMapChaged() {

    if (IsValid(this->LobbyGameMode))
        this->LobbyGameMode->Server_UpdateGameSettings(this->CurrentTextureMap, this->MapName->GetText().ToString());
}

void ULobby::SetMap(UTexture2D* mapImage, FString mapName) {

    this->MapImage->SetVisibility(ESlateVisibility::Visible);
    this->MapImage->SetBrushFromTexture(mapImage);

    this->MapName->SetText(FText::FromString(mapName));

    this->CurrentTextureMap = mapImage;
}

#pragma endregion Map

void ULobby::SetCurrentPlayersFormat(FString InCurrentPlayersFormat) {
    this->PlayerCountButton->SetText(FText::FromString(InCurrentPlayersFormat));
}


void ULobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ULobby, MapImage);
    DOREPLIFETIME(ULobby, MapName);
}