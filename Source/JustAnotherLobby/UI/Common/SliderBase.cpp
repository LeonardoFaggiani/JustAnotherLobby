// Fill out your copyright notice in the Description page of Project Settings.


#include "./SliderBase.h"
#include "Kismet/GameplayStatics.h"

bool USliderBase::Initialize()
{
    if (!Super::Initialize())
        return false;

    this->SetTitle(FText::FromString(this->Title));

    if (VolumeRange)
        VolumeRange->OnValueChanged.AddDynamic(this, &USliderBase::OnVolumeValueChanged);

    return true;
}

void USliderBase::OnVolumeValueChanged(float Value)
{
    if (IsValid(this->SoundMix) && IsValid(this->SoundClass))
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), this->SoundMix, this->SoundClass, Value, 1, 0, true);
}

void USliderBase::SetTitle(FText InTitle)
{
    this->Description->SetText(InTitle);
}

void USliderBase::GetTitle()
{
    this->Description->GetText();
}