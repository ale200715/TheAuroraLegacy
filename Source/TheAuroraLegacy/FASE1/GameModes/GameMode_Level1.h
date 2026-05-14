// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level1.generated.h"

/**
 * 
 */
UCLASS()
class THEAURORALEGACY_API AGameMode_Level1 : public ATheAuroraLegacyGameMode
{
	GENERATED_BODY()

public:
    AGameMode_Level1();
    virtual void BeginPlay() override;
    virtual void SpawnEnemy() override;

private:
    // Pool de enemigos del nivel
    class APhase1EnemyPool* Level1Pool;

    // Encontrar el pool en el nivel
    void FindPool();

    // Cuántos drones han sido spawneados
    int32 TotalSpawned = 0;

    // Máximo de drones activos al mismo tiempo
    int32 MaxActiveAtOnce = 3;
	
};
