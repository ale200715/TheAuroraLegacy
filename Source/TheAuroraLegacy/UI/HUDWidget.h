// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HUDWidget.generated.h"

UCLASS()
class THEAURORALEGACY_API UHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Texto de vida en pantalla
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HealthText;

    // Texto de puntuacion en pantalla
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;

    // Texto de nivel en pantalla
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LevelText;

    // Funciones para actualizar los valores
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetHealth(int32 CurrentHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetScore(int32 NewScore);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetLevel(int32 NewLevel);
};
