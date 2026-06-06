// Fill out your copyright notice in the Description page of Project Settings.

#include "LoreGameMode.h"
#include "LoreWidget.h"
#include "Kismet/GameplayStatics.h"

ALoreGameMode::ALoreGameMode()
{
    // Sin pawn — es solo una pantalla de texto
    DefaultPawnClass = nullptr;
}

void ALoreGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!LoreWidgetClass)
    {
        UE_LOG(LogTemp, Error,
            TEXT("LoreGameMode: LoreWidgetClass no asignado en el nivel."));
        return;
    }

    ULoreWidget* Widget = CreateWidget<ULoreWidget>(
        GetWorld(), LoreWidgetClass);

    if (!Widget) return;

    Widget->SetupLore(LoreText, NextLevelName);
    Widget->AddToViewport();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}

