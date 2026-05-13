#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFacade.generated.h"

// Forward declarations
class UAuroraGameInstance;
class ATheAuroraLegacyGameMode;

// Agregar esto antes de UCLASS()
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    // Fase 1
    Drone,
    Hunter,
    Support,
    // Fase 2
    Assault,
    Maneuver,
    Armored,
    // Fase 3
    Interceptor,
    Turret,
    Boss
};

UCLASS()
class THEAURORALEGACY_API AGameFacade : public AActor
{
    GENERATED_BODY()
public:
    AGameFacade();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void LoseLife();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void SpawnEnemy();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void LoadGame();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void TriggerGameOver();

    // Registrar clase de enemigo
    UFUNCTION(BlueprintCallable, Category = "Facade")
    void RegisterEnemyClass(
        EEnemyType Type,
        TSubclassOf<class AEnemyBase> EnemyClass);

    // Spawnear oleada completa
    UFUNCTION(BlueprintCallable, Category = "Facade")
    void SpawnWave(
        EEnemyType Type,
        int32 Count,
        FVector CenterLocation);

    // Notificar que un enemigo murió
    void NotifyEnemyDefeated(
        class AEnemyBase* Enemy);

    // Limpiar todos los enemigos
    UFUNCTION(BlueprintCallable, Category = "Facade")
    void ClearAllEnemies();

    // Cuántos enemigos han muerto
    UFUNCTION(BlueprintCallable, Category = "Facade")
    int32 GetDefeatedCount() const;

    // Verificar si quedan enemigos
    UFUNCTION(BlueprintCallable, Category = "Facade")
    bool HasActiveEnemies() const;

private:
    UAuroraGameInstance* GetGI();
    ATheAuroraLegacyGameMode* GetGM();
    // Mapa de tipo a clase de enemigo
    TMap<EEnemyType, TSubclassOf<class AEnemyBase>>
        EnemyClasses;

    // Lista de enemigos activos
    UPROPERTY()
    TArray<class AEnemyBase*> ActiveEnemies;

    // Contador de enemigos derrotados
    int32 DefeatedCount = 0;

    // Configurar stats del enemigo según tipo
    void ConfigureEnemy(
        class AEnemyBase* Enemy,
        EEnemyType Type);
};
