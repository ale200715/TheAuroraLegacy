#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFacade.generated.h"

class UAuroraGameInstance;
class ATheAuroraLegacyGameMode;

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

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void RegisterEnemyClass(
        EEnemyType Type,
        TSubclassOf<class AEnemyBase> EnemyClass);

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void SpawnWave(
        EEnemyType Type,
        int32 Count,
        FVector CenterLocation);

    void NotifyEnemyDefeated(
        class AEnemyBase* Enemy);

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void ClearAllEnemies();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    int32 GetDefeatedCount() const;

    UFUNCTION(BlueprintCallable, Category = "Facade")
    bool HasActiveEnemies() const;

private:
    UAuroraGameInstance* GetGI();
    ATheAuroraLegacyGameMode* GetGM();
    TMap<EEnemyType, TSubclassOf<class AEnemyBase>>
        EnemyClasses;

    UPROPERTY()
    TArray<class AEnemyBase*> ActiveEnemies;

    int32 DefeatedCount = 0;

    void ConfigureEnemy(
        class AEnemyBase* Enemy,
        EEnemyType Type);
};
