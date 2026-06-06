// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoreGameMode.generated.h"

UCLASS()
class THEAURORALEGACY_API ALoreGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALoreGameMode();
    virtual void BeginPlay() override;

    // Texto narrativo de la fase — editar en el Blueprint del nivel
    UPROPERTY(EditAnywhere, Category = "Lore")
    FString LoreText;

    // Nivel que carga al presionar Continuar
    UPROPERTY(EditAnywhere, Category = "Lore")
    FName NextLevelName;

    // Título de la fase (ej: "Fase 2 — La Invasión Terrestre")
    UPROPERTY(EditAnywhere, Category = "Lore")
    FString PhaseTitle;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class ULoreWidget> LoreWidgetClass;
};