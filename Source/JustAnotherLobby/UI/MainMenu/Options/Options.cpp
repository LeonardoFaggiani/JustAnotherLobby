// Fill out your copyright notice in the Description page of Project Settings.


#include "Options.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

bool UOptions::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (PreviousTextureQualityButton && NextTextureQualityButton &&
        PreviousResolutionButton && NextResolutionButton &&
        PreviousWindowModeButton && NextWindowModeButton &&
        ApplyButton && BackButton) {

        PreviousTextureQualityButton->OnClicked.AddDynamic(this, &UOptions::OnPreviousTextureQualityButtonClicked);
        NextTextureQualityButton->OnClicked.AddDynamic(this, &UOptions::OnNextTextureQualityButtonClicked);

        PreviousResolutionButton->OnClicked.AddDynamic(this, &UOptions::OnPreviousResolutionButtonClicked);
        NextResolutionButton->OnClicked.AddDynamic(this, &UOptions::OnNextResolutionButtonClicked);

        PreviousWindowModeButton->OnClicked.AddDynamic(this, &UOptions::OnPreviousWindowModeButtonClicked);
        NextWindowModeButton->OnClicked.AddDynamic(this, &UOptions::OnNextWindowModeButtonClicked);

        ApplyButton->OnClicked().AddUObject(this, &UOptions::OnApplyButtonClicked);
        BackButton->OnClicked().AddUObject(this, &UOptions::OnBackButtonClicked);
    }

    UWorld* World = GetWorld();

    if (IsValid(World))
    {
        UGameInstance* GameInstance = World->GetGameInstance();

        if (IsValid(GameInstance))
            JustAnotherLobbyGameInstance = Cast<UJustAnotherLobbyGameInstance>(GameInstance);

    }

    this->InitializeSettings();
    this->InitializeScreenResolutionSupported();
    this->SetDefaultGameSettings();

    return true;
}

void UOptions::InitializeSettings()
{
    this->TextureQualityMaps.Add(0, FText::FromString("Low"));
    this->TextureQualityMaps.Add(1, FText::FromString("Medium"));
    this->TextureQualityMaps.Add(2, FText::FromString("High"));
    this->TextureQualityMaps.Add(3, FText::FromString("Epic"));

    this->WindowModeMaps.Add(0, FText::FromString("Fullscreen"));
    this->WindowModeMaps.Add(1, FText::FromString("Windowed"));

    this->WindowModeIndexMaps.Add(0, EWindowMode::Fullscreen);
    this->WindowModeIndexMaps.Add(1, EWindowMode::Windowed);
}

void UOptions::InitializeScreenResolutionSupported()
{
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(this->SupportedResolutions);

    for (FIntPoint ResolutionItem : this->SupportedResolutions)
    {
        TArray<FStringFormatArg> ScreenResolutionFormats;
        ScreenResolutionFormats.Add(FStringFormatArg(ResolutionItem.X));
        ScreenResolutionFormats.Add(FStringFormatArg(ResolutionItem.Y));

        FString ScreenResolutionCurrentFormat = FString::Format(TEXT("{0}x{1}"), ScreenResolutionFormats);

        int32 ResolutionItemIndex = this->SupportedResolutions.IndexOfByKey(ResolutionItem);

        this->SupportedResolutionsMaps.Add(ResolutionItemIndex, FText::FromString(ScreenResolutionCurrentFormat));
    }
}

void UOptions::SetDefaultGameSettings()
{
    UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

    if (IsValid(GameUserSettings))
    {
        GameUserSettings->LoadSettings();

        this->SetTextureQuality(GameUserSettings->GetTextureQuality());
        this->SetScreenResolution(GameUserSettings->GetScreenResolution());
        this->SetWindowMode(EWindowMode::Fullscreen);
    }
}

void UOptions::SetTextureQuality(int32 InTextureQuality)
{
    this->TextureOption->SetText(this->TextureQualityMaps[InTextureQuality]);
}

void UOptions::SetScreenResolution(FIntPoint InScreenResolution)
{
    if (this->SupportedResolutions.Contains(InScreenResolution)) {

        TArray<FStringFormatArg> ScreenResolutionFormats;
        ScreenResolutionFormats.Add(FStringFormatArg(InScreenResolution.X));
        ScreenResolutionFormats.Add(FStringFormatArg(InScreenResolution.Y));

        FString ScreenResolutionCurrentFormat = FString::Format(TEXT("{0}x{1}"), ScreenResolutionFormats);

        this->ScreenResolutionOption->SetText(FText::FromString(ScreenResolutionCurrentFormat));
    }
}

void UOptions::SetWindowMode(EWindowMode::Type InWindowMode)
{

    auto WindowItemFound = this->WindowModeIndexMaps.FilterByPredicate([&InWindowMode](const TPair<int32, EWindowMode::Type>& item)
        {
            return item.Value == InWindowMode;
        });

    this->WindowModeOption->SetText(this->WindowModeMaps[WindowItemFound.begin()->Key]);
}

void UOptions::OnApplyButtonClicked()
{
    UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

    if (IsValid(GameUserSettings))
    {
        GameUserSettings->LoadSettings();

        GameUserSettings->SetTextureQuality(this->GetTextureIdByValueSelected());
        GameUserSettings->SetScreenResolution(this->GetScreenResolutionByValueSelected());
        GameUserSettings->SetFullscreenMode(this->GetWindowModeByValueSelected());

        GameUserSettings->ApplySettings(false);
    }
}

void UOptions::OnBackButtonClicked()
{
    if (this->JustAnotherLobbyGameInstance != nullptr) {

        if (!this->GetIsOptionMenuInGame()) {
            this->JustAnotherLobbyGameInstance->SetBackToMainMenu(true);
            this->JustAnotherLobbyGameInstance->SetOptionsMenu(false);
            this->JustAnotherLobbyGameInstance->OpenNextLevel("MainMenu", false, false, 0.1f);

            return;
        }

        this->TearDown();
    }
}

void UOptions::OnPreviousTextureQualityButtonClicked()
{
    FText InTextureQualityName = TextureOption->GetText();
    int32 TextureKey = 2;

    for (TPair<int32, FText> TextureQualityItem : this->TextureQualityMaps)
    {
        if (TextureQualityItem.Value.EqualTo(InTextureQualityName)) {

            if ((TextureQualityItem.Key - 1) <= 0)
                TextureKey = 0;
            else
                TextureKey = TextureQualityItem.Key - 1;
        }
    }

    this->SetTextureQuality(TextureKey);
}

void UOptions::OnNextTextureQualityButtonClicked()
{
    FText InTextureQualityName = TextureOption->GetText();
    int32 TextureKey = 2;

    for (TPair<int32, FText> TextureQualityItem : this->TextureQualityMaps)
    {
        if (TextureQualityItem.Value.EqualTo(InTextureQualityName)) {

            if ((TextureQualityItem.Key + 1) >= 3)
                TextureKey = 3;
            else
                TextureKey = TextureQualityItem.Key + 1;
        }
    }

    this->SetTextureQuality(TextureKey);
}

void UOptions::OnPreviousResolutionButtonClicked()
{
    FText InScrenResolutionName = ScreenResolutionOption->GetText();
    int32 ScreenResolutionKey = 0;

    for (TPair<int32, FText> ScreenResolutionItem : this->SupportedResolutionsMaps)
    {
        if (ScreenResolutionItem.Value.EqualTo(InScrenResolutionName)) {
            if ((ScreenResolutionItem.Key - 1) <= 0)
                ScreenResolutionKey = 0;
            else
                ScreenResolutionKey = ScreenResolutionItem.Key - 1;
        }
    }

    this->ScreenResolutionOption->SetText(this->SupportedResolutionsMaps[ScreenResolutionKey]);
}

void UOptions::OnNextResolutionButtonClicked()
{
    FText InScrenResolutionName = ScreenResolutionOption->GetText();
    int32 ScreenResolutionKey = 0;

    for (TPair<int32, FText> ScreenResolutionItem : this->SupportedResolutionsMaps)
    {
        if (ScreenResolutionItem.Value.EqualTo(InScrenResolutionName)) {

            if ((ScreenResolutionItem.Key + 1) >= (this->SupportedResolutionsMaps.Num()))
                ScreenResolutionKey = (this->SupportedResolutionsMaps.Num() - 1);
            else
                ScreenResolutionKey = ScreenResolutionItem.Key + 1;
        }
    }

    this->ScreenResolutionOption->SetText(this->SupportedResolutionsMaps[ScreenResolutionKey]);
}

void UOptions::OnPreviousWindowModeButtonClicked()
{
    FText InWindowModeName = WindowModeOption->GetText();
    int32 WindowModeKey = 0;

    for (TPair<int32, FText> WindowModeItem : this->WindowModeMaps)
    {
        if (WindowModeItem.Value.EqualTo(InWindowModeName)) {

            if ((WindowModeItem.Key - 1) <= 0)
                WindowModeKey = 0;
            else
                WindowModeKey = WindowModeItem.Key - 1;
        }
    }

    this->SetWindowMode(*this->WindowModeIndexMaps.Find(WindowModeKey));
}

void UOptions::OnNextWindowModeButtonClicked()
{
    FText InWindowModeName = WindowModeOption->GetText();
    int32 WindowModeKey = 0;

    for (TPair<int32, FText> WindowModeItem : this->WindowModeMaps)
    {
        if (WindowModeItem.Value.EqualTo(InWindowModeName)) {

            if ((WindowModeItem.Key + 1) >= 1)
                WindowModeKey = 1;
            else
                WindowModeKey = WindowModeItem.Key + 1;
        }
    }

    this->SetWindowMode(*this->WindowModeIndexMaps.Find(WindowModeKey));
}

int32 UOptions::GetTextureIdByValueSelected()
{
    FText InTextureQualityName = TextureOption->GetText();

    int32 TextureQualityId = 0;

    for (TPair<int32, FText> TextureQualityItem : this->TextureQualityMaps)
    {
        if (TextureQualityItem.Value.EqualTo(InTextureQualityName)) {
            TextureQualityId = TextureQualityItem.Key;
        }
    }

    return TextureQualityId;
}

void UOptions::SetIsOptionMenuInGame(bool InbIsOptionMenuInGame)
{
    this->bIsOptionMenuInGame = InbIsOptionMenuInGame;
}

bool UOptions::GetIsOptionMenuInGame()
{
    return this->bIsOptionMenuInGame;
}

FIntPoint UOptions::GetScreenResolutionByValueSelected()
{
    auto InScrenResolutionName = ScreenResolutionOption->GetText().ToString();

    TArray<FString> Substrings;
    FString Delimiter = "x";

    InScrenResolutionName.ParseIntoArray(Substrings, *Delimiter, true);

    FIntPoint IntPoint;
    IntPoint.X = FCString::Atoi(*Substrings[0]);
    IntPoint.Y = FCString::Atoi(*Substrings[1]);

    return IntPoint;
}

EWindowMode::Type UOptions::GetWindowModeByValueSelected()
{
    FText InWindowModeName = WindowModeOption->GetText();
    EWindowMode::Type WindowMode = EWindowMode::Fullscreen;

    for (TPair<int32, FText> WindowModeItem : this->WindowModeMaps)
    {
        if (WindowModeItem.Value.EqualTo(InWindowModeName))
            WindowMode = *this->WindowModeIndexMaps.Find(WindowModeItem.Key);
    }

    return WindowMode;
}
