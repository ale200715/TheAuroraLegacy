// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/AuroraHUD.h"
#include "UI/LoreWidget.h"
//#include "UI/GameOverWidget.h"
#include "TheAuroraLegacyGameMode.generated.h"

UCLASS(MinimalAPI)
class ATheAuroraLegacyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATheAuroraLegacyGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Puntuaci�n
    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 Score = 0;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddScore(int32 Amount);

    // Spawner de enemigos
    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<class AEnemyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 5.f;

    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnDistance = 3000.f;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class ULoreWidget> LoreWidgetClass;

    //UPROPERTY(EditAnywhere, Category = "UI")
    //TSubclassOf<class UGameOverWidget> GameOverWidgetClass;

private:
    FTimerHandle SpawnTimerHandle;
    void SpawnEnemy();
};



