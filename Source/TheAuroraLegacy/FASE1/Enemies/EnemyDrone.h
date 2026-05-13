// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAuroraLegacy/Enemies/EnemyBase.h"
#include "EnemyDrone.generated.h"


UCLASS()
class THEAURORALEGACY_API AEnemyDrone : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemyDrone();
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    // Sobreescribir el movimiento de EnemyBase
    virtual void MoveEnemy(
        float DeltaTime) override;


private:
    // Dirección fija al inicio
    // línea recta sin perseguir al jugador
    FVector MoveDirection;

    // ---- DISPARO ----
    FTimerHandle FireTimerHandle;
    float FireRate = 2.f;
    void FireProjectile();

    // Referencia cacheada al jugador
    // TWeakObjectPtr para no retener en memoria
    TWeakObjectPtr<APawn> CachedPlayer;
    void CachePlayer();
};
