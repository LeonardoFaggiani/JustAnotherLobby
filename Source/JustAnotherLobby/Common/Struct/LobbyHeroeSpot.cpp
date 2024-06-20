// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHeroeSpot.h"

FLobbyHeroeSpot::FLobbyHeroeSpot(int32 InIndex, FLocationSpot InLocationSpot, FLocationHeroe InLocationHeroe)
{
    this->Index = Index;
    this->LocationSpot = InLocationSpot;
    this->LocationHeroe = InLocationHeroe;
}

FLobbyHeroeSpot::FLobbyHeroeSpot()
{

}

FLobbyHeroeSpot::~FLobbyHeroeSpot()
{

}