#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

UCLASS()
class THEAURORALEGACY_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    
    void SetupGameOver(
        int32 LevelWhenDied,
        int32 PhaseWhenDied,
        FName CurrentLevelName);


protected:
    virtual void NativeConstruct() override;

    // Título GAME OVER
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GameOverTitle;

    // Mensaje según la fase
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GameOverMessage;
    
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LevelText;
   
    // Botón reintentar
    UPROPERTY(meta = (BindWidget))
    class UButton* RetryButton;

    // Botón menú principal
    UPROPERTY(meta = (BindWidget))
    class UButton* MainMenuButton;

    // Botón reiniciar desde cero
    UPROPERTY(meta = (BindWidget))
    class UButton* RestartButton;

private:
    FName CurrentLevel;

    UFUNCTION()
    void OnRetryClicked();

    UFUNCTION()
    void OnMainMenuClicked();

    UFUNCTION()
    void OnRestartClicked();

    FString GetMessageForPhase(int32 Phase);
};
