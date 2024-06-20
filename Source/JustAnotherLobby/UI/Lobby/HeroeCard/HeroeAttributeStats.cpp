// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroeAttributeStats.h"

void UHeroeAttributeStats::SetIcon(FString InIconImage)
{
    UTexture2D* IconTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *InIconImage));

    if (IsValid(IconTexture)) 
        this->Icon->SetBrushFromTexture(IconTexture);        
}

void UHeroeAttributeStats::SetPoints(int32 InAttributePoint)
{
    this->Points->SetText(FText::AsNumber(InAttributePoint));
}