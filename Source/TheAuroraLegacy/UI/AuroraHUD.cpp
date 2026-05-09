// Fill out your copyright notice in the Description page of Project Settings.
#include "AuroraHUD.h"

AAuroraHUD::AAuroraHUD()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAuroraHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }
}

void AAuroraHUD::UpdateHealth(int32 CurrentHealth)
{
    if (HUDWidget)
    {
        HUDWidget->SetHealth(CurrentHealth);
    }
}

void AAuroraHUD::UpdateScore(int32 NewScore)
{
    if (HUDWidget)
    {
        HUDWidget->SetScore(NewScore);
    }
}

void AAuroraHUD::UpdateLevel(int32 NewLevel)
{
    if (HUDWidget)
    {
        HUDWidget->SetLevel(NewLevel);
    }
}

