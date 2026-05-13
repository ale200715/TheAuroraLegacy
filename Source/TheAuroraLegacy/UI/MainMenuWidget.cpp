// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Enlazar botones a sus funciones
    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
    }
    if (InstructionsButton)
    {
        InstructionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnInstructionsClicked);
    }
    if (HistoryButton)
    {
        HistoryButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHistoryClicked);
    }
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
    }
}

void UMainMenuWidget::OnPlayClicked()
{
    // Cargar el nivel del juego
    UGameplayStatics::OpenLevel(this, FName("FlyingExampleMap"));
}

void UMainMenuWidget::OnInstructionsClicked()
{
    // Por ahora vacio - se implementara despues
    UE_LOG(LogTemp, Warning, TEXT("Instrucciones clickeado"));
}

void UMainMenuWidget::OnHistoryClicked()
{
    // Por ahora vacio - se implementara despues
    UE_LOG(LogTemp, Warning, TEXT("Historial clickeado"));
}

void UMainMenuWidget::OnQuitClicked()
{
    // Salir del juego
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
