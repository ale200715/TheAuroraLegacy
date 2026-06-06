#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/GameOverWidget.h"
#include "TheAuroraLegacyGameMode.generated.h"

// Forward declarations — no includes innecesarios
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

    // Número de fase: 1, 2 o 3.
    // Cada GameMode hijo lo asigna en su constructor.
    // Lo usa ShowGameOver() para mostrar el mensaje correcto.
    UPROPERTY(EditAnywhere, Category = "Level")
    int32 PhaseNumber = 1;

    // ── Configuración del spawner ─────────────────────────────────

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 5.f;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnDistance = 3000.f;

    // ── UI ────────────────────────────────────────────────────────

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UGameOverWidget> GameOverWidgetClass;

    // Solo Level9 lo asigna en el editor; el resto lo deja vacío.
   UPROPERTY(EditAnywhere, Category = "UI")
   TSubclassOf<UGoodEndingWidget> GoodEndingWidgetClass;

    // ── Timer (los hijos lo usan para cancelarlo si hace falta) ───

    FTimerHandle SpawnTimerHandle;

    // ── Interfaz para los GameModes hijos ─────────────────────────

    // Cada hijo sobreescribe solo esto. El padre maneja todo lo demás.
    virtual void SpawnEnemy();

    // Llamado por el Facade cuando un enemigo muere.
    void OnEnemyDefeated(int32 ScoreValue);

    // Llamado desde el Pawn cuando el jugador muere.
    void OnPlayerDeath();

protected:
    // Contador interno — solo el padre lo modifica.
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

