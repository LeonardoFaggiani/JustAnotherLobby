// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerSpot.h"

FLobbyPlayerSpot::FLobbyPlayerSpot()
{

}

FLobbyPlayerSpot::FLobbyPlayerSpot(int32 InIndex, FVector InLocation, FRotator InRotation)
{
    this->Index = Index;
    this->Location = Location;
    this->Rotation = Rotation;
}

FLobbyPlayerSpot::~FLobbyPlayerSpot()
{

}