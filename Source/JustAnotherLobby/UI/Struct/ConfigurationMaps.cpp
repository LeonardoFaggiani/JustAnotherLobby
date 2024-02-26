// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigurationMaps.h"

FConfigurationMaps::FConfigurationMaps()
{

}

FConfigurationMaps::FConfigurationMaps(FString InPath, FString InImagePath, FString InName, int32 InOrder)
{
    this->Path = InPath;
    this->ImagePath = InImagePath;
    this->Name = InName;
    this->Order = InOrder;
}

FConfigurationMaps::~FConfigurationMaps()
{

}
