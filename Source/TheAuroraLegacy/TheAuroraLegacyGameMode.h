// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/AuroraHUD.h"
#include "UI/LoreWidget.h"
#include "UI/GameOverWidget.h"
#include "TheAuroraLegacyGameMode.generated.h"

UCLASS(MinimalAPI)
class ATheAuroraLegacyGameMode
    : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATheAuroraLegacyGameMode();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ---- PUNTUACION ----
    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 Score = 0;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddScore(int32 Amount);

    // ---- CONTROL DE NIVEL ----
    // Cuántos enemigos hay que matar
    UPROPERTY(EditAnywhere, Category = "Level")
    int32 EnemiesRequired = 5;

    // Cuántos han muerto
    int32 EnemiesDefeated = 0;

    // Siguiente nivel a cargar
    UPROPERTY(EditAnywhere, Category = "Level")
    FName NextLevelName = NAME_None;

    // ---- SPAWNER ----
    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 5.f;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnDistance = 3000.f;

    // ---- UI ----
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class ULoreWidget> LoreWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UGameOverWidget>
        GameOverWidgetClass;

    // ---- FUNCIONES ----
    FTimerHandle SpawnTimerHandle;

    virtual void SpawnEnemy();
    void OnEnemyDefeated(int32 ScoreValue);
    void CheckLevelComplete();
    void LoadNextLevel();
    void OnPlayerDeath();
    void ShowGameOver();
};


