
#include "Phase1EnemyPool.h"
#include "../Enemies/EnemyDrone.h"     
#include "../Projectiles/EnemyProjectile.h"

APhase1EnemyPool::APhase1EnemyPool()
{
    PrimaryActorTick.bCanEverTick = false;

    // Asignar clases directamente en C++
    // sin necesidad de Blueprint
    EnemyClass = AEnemyDrone::StaticClass();
    ProjectileClass =
        AEnemyProjectile::StaticClass();
}

void APhase1EnemyPool::BeginPlay()
{
    Super::BeginPlay();

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