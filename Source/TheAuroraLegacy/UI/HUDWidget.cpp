// Fill out your copyright notice in the Description page of Project Settings.
#include "HUDWidget.h"
#include "Components/TextBlock.h"

void UHUDWidget::SetHealth(int32 CurrentHealth)
{
    if (HealthText)
    {
        HealthText->SetText(FText::FromString("Vida: " + FString::FromInt(CurrentHealth)));
    }
}

void UHUDWidget::SetScore(int32 NewScore)
{
    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString("Score: " + FString::FromInt(NewScore)));
    }
}

void UHUDWidget::SetLevel(int32 NewLevel)
{
    if (LevelText)
    {
        LevelText->SetText(FText::FromString("Nivel: " + FString::FromInt(NewLevel)));
    }
}
