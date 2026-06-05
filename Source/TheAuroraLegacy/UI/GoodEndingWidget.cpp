// Fill out your copyright notice in the Description page of Project Settings.
/*/
#include "GoodEndingWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGoodEndingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (EndingTitle)
        EndingTitle->SetText(FText::FromString(TEXT("La Aurora Prevalece")));

    if (EndingMessage)
        EndingMessage->SetText(FText::FromString(
            TEXT("La Nave Nodriza ha sido destruida.\n" "La invasión ha terminado.\n" "La humanidad se ha salvado gracias a ti.")));

    if (MainMenuButton) {
        MainMenuButton->OnClicked.AddDynamic(this, &UGoodEndingWidget::OnMainMenuClicked);
    }

    // Liberar el control al jugador para que pueda clickear
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void UGoodEndingWidget::OnMainMenuClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}
/*/

#include "GoodEndingWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../AuroraGameInstance.h"

void UGoodEndingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (EndingTitle)
        EndingTitle->SetText(FText::FromString(
            TEXT("La Aurora Prevalece")));

    if (EndingMessage)
        EndingMessage->SetText(FText::FromString(
            TEXT("La Nave Nodriza ha sido destruida.\n"
                "La invasión ha terminado.\n"
                "La humanidad sobrevive gracias a ti.")));

    if (MainMenuButton)
        MainMenuButton->OnClicked.AddDynamic(
            this, &UGoodEndingWidget::OnMainMenuClicked);

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void UGoodEndingWidget::OnMainMenuClicked()
{
    // Al terminar el juego completo reseteamos para que
    // si el jugador vuelve a jugar empiece desde cero
    if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(
        GetGameInstance()))
    {
        GI->ResetStats();
    }

    UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}