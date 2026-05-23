// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Enemies/EnemyBase.h"
#include "Phase3EnemyPool.generated.h"

UCLASS()
class THEAURORALEGACY_API APhase3EnemyPool : public AActor
{
    GENERATED_BODY()

public:
    APhase3EnemyPool();

    // Clase del enemigo a poolear
    UPROPERTY(EditAnywhere, Category = "Pool")
    TSubclassOf<AEnemyBase> EnemyClass;

    // Tamano de la piscina
    UPROPERTY(EditAnywhere, Category = "Pool")
    int32 PoolSize = 15;

    // Llamar desde GameMode despues de asignar EnemyClass
    void InitializePool();

    AEnemyBase* GetPooledEnemy();
    void ReturnToPool(AEnemyBase* Enemy);

protected:
    virtual void BeginPlay() override;

private:
    // Lista de enemigos en la piscina
    TArray<AEnemyBase*> Pool;

    // Inicializar la piscina
    void InitPool();
};