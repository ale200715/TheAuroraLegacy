
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Phase1EnemyPool.generated.h"

UCLASS()
class THEAURORALEGACY_API APhase1EnemyPool : public AActor
{
    GENERATED_BODY()

public:
    APhase1EnemyPool();

protected:
    virtual void BeginPlay() override;

public:

    UPROPERTY(EditAnywhere, Category = "Pool Settings")
    TSubclassOf<class AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Pool Settings")
    int32 PoolSize = 10;
    int32 PoolSizeP = 20;
    TArray<class AEnemyBase*> EnemyPool;

    class AEnemyBase* GetEnemyFromPool();

    UPROPERTY(EditAnywhere, Category = "Pool Settings")
    TSubclassOf<class AEnemyProjectile> ProjectileClass;

    TArray<class AEnemyProjectile*> ProjectilePool;

    class AEnemyProjectile* GetProjectileFromPool();
public:

    void SetEnemyClassBeforeInit(TSubclassOf<AEnemyBase> NewClass)
    {
        EnemyClass = NewClass;
    }
    void InitializePool();
};
