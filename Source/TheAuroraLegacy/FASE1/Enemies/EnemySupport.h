// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemies/EnemyBase.h"
#include "EnemySupport.generated.h"

UCLASS()
class THEAURORALEGACY_API AEnemySupport
    : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemySupport();
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
    float FireRate = 4.f;

private:
    float MoveDirection = 1.f;
    float HorizontalLimit = 700.f;

    float SlowRadius = 500.f;
    bool bIsSlowingPlayer = false;
    float OriginalPlayerSpeed = 0.f;

    void FireProjectile();
    void CheckSlowRadius();

    TWeakObjectPtr<APawn> CachedPlayer;
    void CachePlayer();
    void FindPool();
};
