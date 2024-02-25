// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreen.h"
#include "Animation/UMGSequencePlayer.h"

void ULoadingScreen::TransBounceIn()
{
    this->PlayAnimationForward(this->BounceInOut);
}

void ULoadingScreen::TransBounceInCompleted()
{
    this->PlayAnimationForward(this->BounceInCompleted);
}

void ULoadingScreen::TransBounceOut()
{
    FTimerHandle MemberTimerHandle;

    this->PlayAnimationReverse(this->BounceInOut);

    GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &ThisClass::RemoveFromParent, this->BounceInOut->GetEndTime(), false);
}