#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "TankEnemy.generated.h"

// Forward declaration
class IMovementStrategy;
class ZigZagStrategy;

UCLASS()
class THEAURORALEGACY_API ATankEnemy : public AEnemyBase
{
    GENERATED_BODY()

public:
    ATankEnemy();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float FireRate = 1.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<class APhase2EnemyProjectile> ProjectileClass;;

private:
    // Strategy pattern
    IMovementStrategy* MovementStrategy;
    ZigZagStrategy* ZigZag;

    FTimerHandle FireTimerHandle;
    void FireAtPlayer();
    void NotifyDeath();

    UFUNCTION()
    void OnTankDestroyed(AActor* DestroyedActor);
};
