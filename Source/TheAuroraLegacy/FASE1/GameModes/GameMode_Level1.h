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
    class APhase1EnemyPool* Level1Pool;

    void FindPool();

    int32 TotalSpawned = 0;

    int32 MaxActiveAtOnce = 3;
	
};
