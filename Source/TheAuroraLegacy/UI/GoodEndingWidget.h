// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoodEndingWidget.generated.h"

UCLASS()
class THEAURORALEGACY_API UGoodEndingWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // Título del ending — BindWidget: el nombre debe coincidir exactamente en el WBP
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EndingTitle;

    // Mensaje narrativo
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EndingMessage;

    // Único botón: volver al menú principal
    UPROPERTY(meta = (BindWidget))
    class UButton* MainMenuButton;

private:
    UFUNCTION()
    void OnMainMenuClicked();
};