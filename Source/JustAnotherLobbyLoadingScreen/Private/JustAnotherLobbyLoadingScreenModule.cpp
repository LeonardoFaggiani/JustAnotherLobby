// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JustAnotherLobbyLoadingScreenModule.h"
#include "JustAnotherLobbyLoadingScreen.h"
#include <MoviePlayer.h>

class FJustAnotherLobbyLoadingScreenModule : public IJustAnotherLobbyLoadingScreenModule
{	

public:
	virtual void StartupModule() override
	{
		//Force load for cooker reference
		this->LoadingScreenImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Common/Textures/LoadingScreenParagon.LoadingScreenParagon"));
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void ShowLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		if(!IsValid(this->LoadingScreenImage))
			this->LoadingScreenImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Common/Textures/LoadingScreenParagon.LoadingScreenParagon"));

		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(SJustAnotherLobbyLoadingScreen).Image(LoadingScreenImage);

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

	virtual void HideLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}
};

IMPLEMENT_MODULE(FJustAnotherLobbyLoadingScreenModule, JustAnotherLobbyLoadingScreen);
