/*
#include "Phase1EnemyPool.h"
#include "../Enemies/EnemyDrone.h"
#include "../Projectiles/EnemyProjectile.h"

APhase1EnemyPool::APhase1EnemyPool()
{
    PrimaryActorTick.bCanEverTick = false;
    ProjectileClass =
        AEnemyProjectile::StaticClass();
    PoolSize = 20;
}

void APhase1EnemyPool::BeginPlay()
{
    Super::BeginPlay();

    // Solo crear enemigos si hay clase asignada
    // El GameMode asigna la clase antes con
    // ReinitializePool
    if (EnemyClass)
    {
        for (int32 i = 0; i < PoolSize; i++)
        {
            AEnemyBase* NewEnemy =
                GetWorld()->SpawnActor<AEnemyBase>(
                    EnemyClass,
                    FVector::ZeroVector,
                    FRotator::ZeroRotator);

            if (NewEnemy)
            {
                NewEnemy->SetActorHiddenInGame(true);
                NewEnemy->SetActorTickEnabled(false);
                NewEnemy->SetActorEnableCollision(
                    false);
                EnemyPool.Add(NewEnemy);
            }
        }
    }

    // Proyectiles siempre se inicializan
    for (int32 i = 0; i < 20; i++)
    {
        if (ProjectileClass)
        {
            AEnemyProjectile* NewProj =
                GetWorld()->SpawnActor
                <AEnemyProjectile>(
                    ProjectileClass,
                    FVector::ZeroVector,
                    FRotator::ZeroRotator);

            if (NewProj)
            {
                NewProj->SetActorHiddenInGame(true);
                NewProj->SetActorTickEnabled(false);
                NewProj->SetActorEnableCollision(
                    false);
                ProjectilePool.Add(NewProj);
            }
        }
    }
}

AEnemyBase* APhase1EnemyPool::GetEnemyFromPool()
{
    for (AEnemyBase* Enemy : EnemyPool)
    {
        if (Enemy && Enemy->IsHidden())
            return Enemy;
    }
    return nullptr;
}

AEnemyProjectile*
APhase1EnemyPool::GetProjectileFromPool()
{
    for (AEnemyProjectile* Proj : ProjectilePool)
    {
        if (Proj && Proj->IsHidden())
            return Proj;
    }
    return nullptr;
}

void APhase1EnemyPool::ReinitializePool(
    TSubclassOf<AEnemyBase> NewEnemyClass)
{
    for (AEnemyBase* Enemy : EnemyPool)
        if (Enemy) Enemy->Destroy();

    EnemyPool.Empty();
    EnemyClass = NewEnemyClass;

    for (int32 i = 0; i < PoolSize; i++)
    {
        if (!EnemyClass) continue;

        AEnemyBase* NewEnemy =
            GetWorld()->SpawnActor<AEnemyBase>(
                EnemyClass,
                FVector::ZeroVector,
                FRotator::ZeroRotator);

        if (NewEnemy)
        {
            NewEnemy->SetActorHiddenInGame(true);
            NewEnemy->SetActorTickEnabled(false);
            NewEnemy->SetActorEnableCollision(false);
            EnemyPool.Add(NewEnemy);
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Pool: Reinicializado con %d enemigos"),
        EnemyPool.Num());
}

*/


#include "Phase1EnemyPool.h"
#include "../Enemies/EnemyDrone.h"     
#include "../Projectiles/EnemyProjectile.h"

APhase1EnemyPool::APhase1EnemyPool()
{
    PrimaryActorTick.bCanEverTick = false;

    // Asignar clases directamente en C++
    // sin necesidad de Blueprint
    //EnemyClass = AEnemyDrone::StaticClass();
    ProjectileClass =
        AEnemyProjectile::StaticClass();
    PoolSize = 20;
}

void APhase1EnemyPool::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle InitTimer;
    GetWorldTimerManager().SetTimer(
        InitTimer,
        this,
        &APhase1EnemyPool::InitializePool,
        0.1f,
        false);
    // 1. Pre-cargar Enemigos
    for (int32 i = 0; i < PoolSize; i++)
    {
        if (EnemyClass)
        {
            AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass, FVector::ZeroVector, FRotator::ZeroRotator);
            if (NewEnemy)
            {
                NewEnemy->SetActorHiddenInGame(true);
                NewEnemy->SetActorTickEnabled(false);
                NewEnemy->SetActorEnableCollision(false);
                EnemyPool.Add(NewEnemy);
            }
        }
    }

    // 2. Pre-cargar Proyectiles (haremos 20 para que alcancen para todos los drones)
    for (int32 i = 0; i < 20; i++)
    {
        if (ProjectileClass)
        {
            AEnemyProjectile* NewProj = GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator);
            if (NewProj)
            {
                NewProj->SetActorHiddenInGame(true);
                NewProj->SetActorTickEnabled(false);
                NewProj->SetActorEnableCollision(false);
                ProjectilePool.Add(NewProj);
            }
        }
    }
}

// Función maestra: busca un enemigo que no esté activo
AEnemyBase* APhase1EnemyPool::GetEnemyFromPool()
{
    for (AEnemyBase* Enemy : EnemyPool)
    {
        if (Enemy && Enemy->IsHidden()) // Si está oculto, está disponible
        {
            return Enemy;
        }
    }
    return nullptr; // Si todos están ocupados
}

AEnemyProjectile* APhase1EnemyPool::GetProjectileFromPool()
{
    for (AEnemyProjectile* Proj : ProjectilePool)
    {
        if (Proj && Proj->IsHidden())
        {
            return Proj;
        }
    }
    return nullptr;
}

void APhase1EnemyPool::InitializePool()
{
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error,
            TEXT("Pool: No hay EnemyClass asignada"));
        return;
    }

    EnemyPool.Empty();

    for (int32 i = 0; i < PoolSize; i++)
    {
        AEnemyBase* NewEnemy =
            GetWorld()->SpawnActor<AEnemyBase>(
                EnemyClass,
                FVector::ZeroVector,
                FRotator::ZeroRotator);

        if (NewEnemy)
        {
            NewEnemy->SetActorHiddenInGame(true);
            NewEnemy->SetActorTickEnabled(false);
            NewEnemy->SetActorEnableCollision(false);
            EnemyPool.Add(NewEnemy);
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Pool: Inicializado con %d enemigos "
            "de clase %s"),
        EnemyPool.Num(),
        *EnemyClass->GetName());
}

void APhase1EnemyPool::ReinitializePool(
    TSubclassOf<AEnemyBase> NewEnemyClass)
{
    // Destruir enemigos anteriores
    for (AEnemyBase* Enemy : EnemyPool)
    {
        if (Enemy) Enemy->Destroy();
    }
    EnemyPool.Empty();

    // Crear nuevos con la clase correcta
    EnemyClass = NewEnemyClass;
    for (int32 i = 0; i < PoolSize; i++)
    {
        if (!EnemyClass) continue;

        AEnemyBase* NewEnemy =
            GetWorld()->SpawnActor<AEnemyBase>(
                EnemyClass,
                FVector::ZeroVector,
                FRotator::ZeroRotator);

        if (NewEnemy)
        {
            NewEnemy->SetActorHiddenInGame(true);
            NewEnemy->SetActorTickEnabled(false);
            NewEnemy->SetActorEnableCollision(false);
            EnemyPool.Add(NewEnemy);
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Pool: Reinicializado con %d enemigos"),
        EnemyPool.Num());
}