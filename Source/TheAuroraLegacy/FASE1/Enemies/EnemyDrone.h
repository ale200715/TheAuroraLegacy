#pragma once
#include "CoreMinimal.h"
#include "TheAuroraLegacy/Enemies/EnemyBase.h"
#include "EnemyDrone.generated.h"

UCLASS()
class THEAURORALEGACY_API AEnemyDrone
    : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemyDrone();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* EnemyMesh;
    void RestartFireTimer();

protected:
    virtual void BeginPlay() override;
    virtual void MoveEnemy(float DeltaTime) override;
    virtual void OnDeath() override;
    class APhase1EnemyPool* LevelPool;
    FTimerHandle FireTimerHandle;
    float FireRate = 2.0f;

private:
    FVector MoveDirection;
    void FireProjectile();
    TWeakObjectPtr<APawn> CachedPlayer;
    void CachePlayer();
};