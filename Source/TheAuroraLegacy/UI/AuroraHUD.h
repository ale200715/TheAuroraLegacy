// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDWidget.h"
#include "AuroraHUD.generated.h"

UCLASS()
class THEAURORALEGACY_API AAuroraHUD : public AHUD
{
    GENERATED_BODY()

public:
    AAuroraHUD();

    virtual void BeginPlay() override;

    // Clase del widget a crear
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UHUDWidget> HUDWidgetClass;

    // Referencia al widget activo
    UPROPERTY()
    UHUDWidget* HUDWidget;

    // Actualizar vida en pantalla
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(int32 CurrentHealth);

    // Actualizar puntuacion en pantalla
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateScore(int32 NewScore);

    // Actualizar nivel en pantalla
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateLevel(int32 NewLevel);
};
