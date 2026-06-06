#include "LoreWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void ULoreWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &ULoreWidget::OnContinueClicked);
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void ULoreWidget::SetupLore(FString Text,FName NextLevelName)
{
    NextLevel = NextLevelName;

    if (LoreText)
    {
        LoreText->SetText( FText::FromString(Text));
    }
}

void ULoreWidget::OnContinueClicked()
{
    APlayerController* PC =GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }

    if (NextLevel != NAME_None)
    {
        UGameplayStatics::OpenLevel(this, NextLevel);
    }
}