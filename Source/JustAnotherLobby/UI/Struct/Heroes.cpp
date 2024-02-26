// Fill out your copyright notice in the Description page of Project Settings.


#include "Heroes.h"

FHeroes::FHeroes(FString InName,
    FString InIcon,
    TSubclassOf<ACharacterBase> InTargetClass,
    TArray<struct FHeroeAttribute> InAttributes)
{
    this->Name = InName;    
    this->Icon = InIcon;
    this->TargetClass = InTargetClass;
    this->Attributes = InAttributes;
}

FHeroes::FHeroes()
{
}

FHeroes::~FHeroes()
{
}