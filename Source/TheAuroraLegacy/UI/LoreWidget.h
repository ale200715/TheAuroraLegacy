#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoreWidget.generated.h"

UCLASS()
class THEAURORALEGACY_API ULoreWidget
    : public UUserWidget
{
    GENERATED_BODY()

public:
    // Configurar el texto y nivel siguiente
    // antes de mostrar la pantalla
    void SetupLore(
        FString Text,
        FName NextLevelName);

protected:
    virtual void NativeConstruct() override;

    // Texto del lore
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LoreText;

    // Subtítulo de fase
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PhaseTitle;

    // Botón para continuar
    UPROPERTY(meta = (BindWidget))
    class UButton* ContinueButton;

private:
    FName NextLevel;

    UFUNCTION()
    void OnContinueClicked();
};
