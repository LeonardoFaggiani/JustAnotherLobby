// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonBase.h"

bool UButtonBase::Initialize()
{
    if (!Super::Initialize())
        return false;

    this->SetText(FText::FromString(this->ButtonText));

    return true;
}

void UButtonBase::SetText(FText InText)
{

    this->ButtonTitle->SetText(InText);

}
