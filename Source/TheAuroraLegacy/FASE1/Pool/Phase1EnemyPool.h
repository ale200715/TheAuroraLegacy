
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
    // --- SECCIÓN DE ENEMIGOS ---
    UPROPERTY(EditAnywhere, Category = "Pool Settings")
    TSubclassOf<class AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Pool Settings")
    int32 PoolSize = 10;

    // Usamos punteros para no duplicar datos
    TArray<class AEnemyBase*> EnemyPool;

    // Función para obtener un enemigo disponible
    class AEnemyBase* GetEnemyFromPool();

    // --- SECCIÓN DE PROYECTILES ---
    UPROPERTY(EditAnywhere, Category = "Pool Settings")
    TSubclassOf<class AEnemyProjectile> ProjectileClass;

    TArray<class AEnemyProjectile*> ProjectilePool;

    class AEnemyProjectile* GetProjectileFromPool();
public:
    void ReinitializePool(
        TSubclassOf<AEnemyBase> NewEnemyClass);
};
