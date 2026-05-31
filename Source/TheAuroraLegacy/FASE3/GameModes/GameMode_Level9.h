// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level9.generated.h"

UCLASS()
class THEAURORALEGACY_API AGameMode_Level9
    : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()

public:
    AGameMode_Level9();

protected:
    virtual void BeginPlay() override;
    virtual void SpawnEnemy() override;

private:
    bool bBossSpawned = false;
};
