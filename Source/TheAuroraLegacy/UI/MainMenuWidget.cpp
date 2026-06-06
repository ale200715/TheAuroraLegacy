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

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(
            this, &UMainMenuWidget::OnQuitClicked);
}

void UMainMenuWidget::OnPlayClicked()
{
    if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(GetGameInstance()))
        GI->ResetStats();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }

    UGameplayStatics::OpenLevel(this, FName("Lore_Fase1"));
}

void UMainMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(
        this, nullptr, EQuitPreference::Quit, false);
}