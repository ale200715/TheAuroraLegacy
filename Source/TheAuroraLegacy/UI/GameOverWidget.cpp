#include "GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RetryButton)
        RetryButton->OnClicked.AddDynamic(
            this, &UGameOverWidget::OnRetryClicked);

    if (MainMenuButton)
        MainMenuButton->OnClicked.AddDynamic(
            this, &UGameOverWidget::OnMainMenuClicked);

    if (RestartButton)
        RestartButton->OnClicked.AddDynamic(
            this, &UGameOverWidget::OnRestartClicked);

    // Mostrar cursor para los botones
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void UGameOverWidget::SetupGameOver(
    int32 LevelWhenDied,
    int32 PhaseWhenDied,
    FName CurrentLevelName)
{
    CurrentLevel = CurrentLevelName;

    if (LevelText)
    {
        FString Text = FString::Printf(
            TEXT("Caíste en el Nivel %d"),
            LevelWhenDied);
        LevelText->SetText(
            FText::FromString(Text));
    }

    if (GameOverMessage)
    {
        GameOverMessage->SetText(
            FText::FromString(
                GetMessageForPhase(PhaseWhenDied)));
    }
}

FString UGameOverWidget::GetMessageForPhase(
    int32 Phase)
{
    switch (Phase)
    {
    case 1:
        return TEXT(
            "La primera línea de defensa "
            "ha caído.\n"
            "Los invasores avanzan sin "
            "resistencia.\n"
            "La Tierra necesita un nuevo héroe.");
    case 2:
        return TEXT(
            "Las ciudades han sido tomadas.\n"
            "La humanidad ha perdido sus "
            "últimos bastiones.\n"
            "El silencio cubre la Tierra.");
    case 3:
        return TEXT(
            "La Nave Nodriza prevalece.\n"
            "La oscuridad cubre el planeta.\n"
            "La invasión ha sido completada.");
    default:
        return TEXT(
            "La Tierra ha caído.\n"
            "La misión ha fracasado.");
    }
}

void UGameOverWidget::OnRetryClicked()
{
    APlayerController* PC =
        GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }

    UGameplayStatics::OpenLevel(
        this, CurrentLevel);
}

void UGameOverWidget::OnMainMenuClicked()
{
    UGameplayStatics::OpenLevel(
        this, FName("MainMenu"));
}

void UGameOverWidget::OnRestartClicked()
{
    UGameplayStatics::OpenLevel(
        this, FName("Level1_Scout"));
}