// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemies/EnemyBase.h"
#include "EnemyHunter.generated.h"

UCLASS()
class THEAURORALEGACY_API AEnemyHunter
    : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemyHunter();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* EnemyMesh;

protected:
    virtual void BeginPlay() override;
    virtual void MoveEnemy(float DeltaTime) override;
    virtual void OnDeath() override;

    class APhase1EnemyPool* LevelPool;
    FTimerHandle FireTimerHandle;
    FTimerHandle BurstTimerHandle;
    float FireRate = 1.5f;
    int32 BurstCount = 0;
    int32 BurstMax = 2;

private:
    TWeakObjectPtr<APawn> CachedPlayer;
    void CachePlayer();
    void StartBurst();
    void FireBurst();
    void FindPool();
};
