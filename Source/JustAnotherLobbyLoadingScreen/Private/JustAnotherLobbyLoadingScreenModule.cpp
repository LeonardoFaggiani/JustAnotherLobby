// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JustAnotherLobbyLoadingScreenModule.h"

class FJustAnotherLobbyLoadingScreenModule : public IJustAnotherLobbyLoadingScreenModule
{	
	virtual bool IsGameModule() const override
	{
		return true;
	}
};

IMPLEMENT_MODULE(FJustAnotherLobbyLoadingScreenModule, JustAnotherLobbyLoadingScreen);
