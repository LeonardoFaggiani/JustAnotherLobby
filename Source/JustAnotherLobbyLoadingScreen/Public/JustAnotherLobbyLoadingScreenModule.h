// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
//
//#include "ModuleInterface.h"
#include "Modules/ModuleManager.h"

/** Module interface for this game's loading screens */
class IJustAnotherLobbyLoadingScreenModule : public IModuleInterface
{
public:
	/** Loads the module so it can be turned on */
	static inline IJustAnotherLobbyLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IJustAnotherLobbyLoadingScreenModule>("JustAnotherLobbyLoadingScreen");
	}
};
