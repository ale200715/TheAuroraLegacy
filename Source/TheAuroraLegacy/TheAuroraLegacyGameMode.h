#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/GameOverWidget.h"
#include "TheAuroraLegacyGameMode.generated.h"

class AGameFacade;
class ULoreWidget;
class UGoodEndingWidget;

UCLASS(MinimalAPI)
class ATheAuroraLegacyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATheAuroraLegacyGameMode();
    virtual void BeginPlay() override;


    UPROPERTY(EditAnywhere, Category = "Level")
    int32 EnemiesRequired = 5;

    UPROPERTY(EditAnywhere, Category = "Level")
    FName NextLevelName = NAME_None;

    UPROPERTY(EditAnywhere, Category = "Level")
    int32 PhaseNumber = 1;


    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 5.f;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnDistance = 3000.f;


    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UGameOverWidget> GameOverWidgetClass;

   UPROPERTY(EditAnywhere, Category = "UI")
   TSubclassOf<UGoodEndingWidget> GoodEndingWidgetClass;

    FTimerHandle SpawnTimerHandle;
    virtual void SpawnEnemy();
    void OnEnemyDefeated(int32 ScoreValue);
    void OnPlayerDeath();

protected:
    int32 EnemiesDefeated = 0;
    int32 TotalSpawned = 0;      
    int32 MaxActiveAtOnce = 3;
    UPROPERTY(BlueprintReadOnly, Category = "Facade")
    AGameFacade* GameFacadeInstance;
private:
    void CheckLevelComplete();
    void LoadNextLevel();
    void ShowGameOver();
    void ShowGoodEnding();
};

