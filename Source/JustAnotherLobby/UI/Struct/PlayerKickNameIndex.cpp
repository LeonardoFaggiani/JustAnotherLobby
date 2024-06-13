// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerKickNameIndex.h"

FPlayerKickNameIndex::FPlayerKickNameIndex(int32 InIndex, FText InName, bool bIsReady)
{
    this->Index = InIndex;
    this->Name = InName;
    this->IsReady = bIsReady;
}

FPlayerKickNameIndex::FPlayerKickNameIndex()
{
}

FPlayerKickNameIndex::~FPlayerKickNameIndex()
{
}
