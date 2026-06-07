// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase3EnemyPool.h"
#include "../Enemies/Enemy_Torreta.h"
#include "../Interceptor.h"

APhase3EnemyPool::APhase3EnemyPool()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APhase3EnemyPool::BeginPlay()
{
    Super::BeginPlay();
   
}

void APhase3EnemyPool::InitializePool()
{
    InitPool();
}

void APhase3EnemyPool::InitPool()
{
    if (!EnemyClass) return;

    for (int32 i = 0; i < PoolSize; i++)
    {
        // Spawnear enemigo inactivo
        AEnemyBase* Enemy = GetWorld()->SpawnActor<AEnemyBase>(
            EnemyClass,
            FVector::ZeroVector,
            FRotator::ZeroRotator);

        if (Enemy)
        {
            // Desactivarlo al inicio
            Enemy->SetActorHiddenInGame(true);
            Enemy->SetActorEnableCollision(false);
            Enemy->SetActorTickEnabled(false);
            Pool.Add(Enemy);
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Pool inicializado con %d enemigos"),
        Pool.Num());
}

AEnemyBase* APhase3EnemyPool::GetPooledEnemy()
{
    // Buscar un enemigo inactivo
    for (AEnemyBase* Enemy : Pool)
    {
        if (Enemy && Enemy->IsHidden())
        {
            // Activarlo
            Enemy->SetActorHiddenInGame(false);
            Enemy->SetActorEnableCollision(true);
            Enemy->SetActorTickEnabled(true);

            // Asignar referencia al pool en el Interceptor
            AInterceptor* Interceptor =
                Cast<AInterceptor>(Enemy);
            if (Interceptor)
            {
                Interceptor->OwnerPool = this;
            }

            // Iniciar comportamiento especifico
            // de la torreta al activarse
            AEnemy_Torreta* Torreta =
                Cast<AEnemy_Torreta>(Enemy);
            if (Torreta)
            {
                Torreta->ActivateTurret();
            }

            return Enemy;
        }
    }

    // Si no hay disponibles retorna null
    UE_LOG(LogTemp, Warning,
        TEXT("Pool vacio, no hay enemigos disponibles"));
    return nullptr;
}

void APhase3EnemyPool::ReturnToPool(AEnemyBase* Enemy)
{
    if (!Enemy) return;

    // Desactivarlo en lugar de destruirlo
    Enemy->SetActorHiddenInGame(true);
    Enemy->SetActorEnableCollision(false);
    Enemy->SetActorTickEnabled(false);
    Enemy->SetActorLocation(FVector::ZeroVector);

    UE_LOG(LogTemp, Warning,
        TEXT("Enemigo devuelto al pool"));
}

