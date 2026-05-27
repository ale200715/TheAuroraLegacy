#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_Level4.generated.h"

// Observer Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChanged, int32, RemainingEnemies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyGroupSpawned, int32, GroupSize);

UCLASS()
class THEAURORALEGACY_API AGameMode_Level4 : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGameMode_Level4();
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class AInfantryEnemy> EnemyClass;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 EnemiesDefeated = 0;

    UPROPERTY(EditAnywhere, Category = "Stats")
    int32 EnemiesToDefeat = 10;

    UFUNCTION()
    void OnEnemyDefeated();

    // Observer — el HUD o cualquier clase puede suscribirse
    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnEnemyCountChanged OnEnemyCountChanged;

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnEnemyGroupSpawned OnEnemyGroupSpawned;

private:
    FTimerHandle SpawnTimerHandle;
    int32 ActiveEnemies = 0;
    void SpawnEnemyGroup();
};