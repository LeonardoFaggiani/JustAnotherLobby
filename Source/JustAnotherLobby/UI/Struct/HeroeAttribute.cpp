// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeAttribute.h"

FHeroeAttribute::FHeroeAttribute(FString InIcon,
    int32 InPoints,
    TEnumAsByte<EAttributeType> InAttributeType)
{
    this->Icon = InIcon;
    this->Points = InPoints;
    this->AttributeType = InAttributeType;
}

FHeroeAttribute::FHeroeAttribute()
{
}

FHeroeAttribute::~FHeroeAttribute()
{

}

