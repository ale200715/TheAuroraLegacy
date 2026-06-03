// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/AuroraHUD.h"
#include "UI/LoreWidget.h"
#include "UI/GameOverWidget.h"
#include "TheAuroraLegacyGameMode.generated.h"

class AGameFacade;
UCLASS(MinimalAPI)
class ATheAuroraLegacyGameMode: public AGameModeBase
{
    GENERATED_BODY()

public:
    ATheAuroraLegacyGameMode();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 Score = 0;

    UPROPERTY(EditAnywhere, Category = "Level")
    int32 EnemiesRequired = 5;

    int32 EnemiesDefeated = 0;

    UPROPERTY(EditAnywhere, Category = "Level")
    FName NextLevelName = NAME_None;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 5.f;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnDistance = 3000.f;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class ULoreWidget> LoreWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UGameOverWidget>
        GameOverWidgetClass;

    FTimerHandle SpawnTimerHandle;

    virtual void SpawnEnemy();
    void OnEnemyDefeated(int32 ScoreValue);
    void CheckLevelComplete();
    void LoadNextLevel();
    void OnPlayerDeath();
    void ShowGameOver();
protected:
    UPROPERTY(BlueprintReadOnly, Category = "Facade")
    AGameFacade* GameFacadeInstance;
};


