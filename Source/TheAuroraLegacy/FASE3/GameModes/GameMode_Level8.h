// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "../Pool/Phase3EnemyPool.h"
#include "GameMode_Level8.generated.h"

UCLASS()
class THEAURORALEGACY_API AGameMode_Level8
    : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()

public:
    AGameMode_Level8();

protected:
    virtual void BeginPlay() override;
    virtual void SpawnEnemy() override;

private:
    int32 EnemiesSpawned = 0;
    int32 MaxEnemies = 5; // segun documento son 5 torretas

    UPROPERTY()
    APhase3EnemyPool* EnemyPool;

    // Posiciones fijas donde apareceran las torretas
    TArray<FVector> SpawnPositions;
};
