// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level2.generated.h"

UCLASS()
class THEAURORALEGACY_API AGameMode_Level2 : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()

public:
    AGameMode_Level2();
    virtual void BeginPlay() override;
    virtual void SpawnEnemy() override;

private:
    class APhase1EnemyPool* LevelPool;
};