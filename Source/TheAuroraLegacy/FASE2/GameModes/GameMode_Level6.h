#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_Level6.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyerCountChanged, int32, RemainingDestroyers);

UCLASS()
class THEAURORALEGACY_API AGameMode_Level6 : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGameMode_Level6();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class ADestroyerEnemy> EnemyClass;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 EnemiesDefeated = 0;

    UPROPERTY(EditAnywhere, Category = "Stats")
    int32 EnemiesToDefeat = 5;

    UFUNCTION()
    void OnEnemyDefeated();

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnDestroyerCountChanged OnDestroyerCountChanged;

private:
    FTimerHandle SpawnTimerHandle;
    void SpawnDestroyer();
};
