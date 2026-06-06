// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../AuroraGameInstance.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PlayButton)
        PlayButton->OnClicked.AddDynamic(
            this, &UMainMenuWidget::OnPlayClicked);

    if (InstructionsButton)
        InstructionsButton->OnClicked.AddDynamic(
            this, &UMainMenuWidget::OnInstructionsClicked);

    if (HistoryButton)
        HistoryButton->OnClicked.AddDynamic(
            this, &UMainMenuWidget::OnHistoryClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(
            this, &UMainMenuWidget::OnQuitClicked);
}

void UMainMenuWidget::OnPlayClicked()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly()); // ← esto falta
    }

    if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(
        GetGameInstance()))
    {
        GI->ResetStats();
    }

    UGameplayStatics::OpenLevel(this, FName("Level1_Drone"));
}

void UMainMenuWidget::OnInstructionsClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Instrucciones clickeado"));
}

void UMainMenuWidget::OnHistoryClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Historial clickeado"));
}

void UMainMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(
        this, nullptr, EQuitPreference::Quit, false);
}