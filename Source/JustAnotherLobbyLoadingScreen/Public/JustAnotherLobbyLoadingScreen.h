// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class JUSTANOTHERLOBBYLOADINGSCREEN_API SJustAnotherLobbyLoadingScreen : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SJustAnotherLobbyLoadingScreen) : _Image(nullptr) {}
	SLATE_ARGUMENT(UTexture2D*, Image);
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	UTexture2D* Image;

	/** Loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;

private:
	EVisibility GetLoadIndicatorVisibility() const;
};
