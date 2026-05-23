// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "../Pool/Phase3EnemyPool.h"
#include "GameMode_Level7.generated.h"

UCLASS()
class THEAURORALEGACY_API AGameMode_Level7
    : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()

public:
    AGameMode_Level7();

protected:
    virtual void BeginPlay() override;
    virtual void SpawnEnemy() override;

private:
    int32 EnemiesSpawned = 0;
    int32 MaxEnemies = 15;

    // Referencia al pool
    UPROPERTY()
    APhase3EnemyPool* EnemyPool;
};