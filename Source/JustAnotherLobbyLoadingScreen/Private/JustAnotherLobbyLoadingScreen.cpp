// Fill out your copyright notice in the Description page of Project Settings.


#include "JustAnotherLobbyLoadingScreen.h"
#include "SlateOptMacros.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Brushes/SlateImageBrush.h"
#include "MoviePlayer.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SJustAnotherLobbyLoadingScreen::Construct(const FArguments& InArgs)
{
	// Load version of the logo with text baked in, path is hardcoded because this loads very early in startup
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());		

	if (!InArgs._Image)
		return; //No Texture Loaded

	InArgs._Image->UpdateResource();

	FSlateImageBrush* ImageBrush = new FSlateImageBrush(InArgs._Image, ViewportSize);

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SImage)
						.Image(ImageBrush)
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.VAlign(VAlign_Bottom)
						.HAlign(HAlign_Right)
						.Padding(FMargin(10.0f))
						[
							SNew(SThrobber)
								.Visibility(this, &SJustAnotherLobbyLoadingScreen::GetLoadIndicatorVisibility)
						]
				]
		];
}

EVisibility SJustAnotherLobbyLoadingScreen::GetLoadIndicatorVisibility() const
{
	return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
