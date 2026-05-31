#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_Level5.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTankCountChanged, int32, RemainingTanks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTankGroupSpawned, int32, GroupSize);

UCLASS()
class THEAURORALEGACY_API AGameMode_Level5 : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGameMode_Level5();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class ATankEnemy> EnemyClass;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 EnemiesDefeated = 0;

    UPROPERTY(EditAnywhere, Category = "Stats")
    int32 EnemiesToDefeat = 8;

    UFUNCTION()
    void OnEnemyDefeated();

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnTankCountChanged OnTankCountChanged;

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnTankGroupSpawned OnTankGroupSpawned;

private:
    FTimerHandle SpawnTimerHandle;
    void SpawnTank();
};
