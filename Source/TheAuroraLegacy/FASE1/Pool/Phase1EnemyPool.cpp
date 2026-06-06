#include "Phase1EnemyPool.h"
#include "../Enemies/EnemyDrone.h"     
#include "../Projectiles/EnemyProjectile.h"

APhase1EnemyPool::APhase1EnemyPool()
{
    PrimaryActorTick.bCanEverTick = false;

    ProjectileClass = AEnemyProjectile::StaticClass();
}

void APhase1EnemyPool::BeginPlay()
{
    Super::BeginPlay();
    
    for (int32 i = 0; i < PoolSizeP ; i++)
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

AEnemyBase* APhase1EnemyPool::GetEnemyFromPool()
{
    for (AEnemyBase* Enemy : EnemyPool)
    {
        if (Enemy && Enemy->IsHidden()) 
        {
            return Enemy;
        }
    }
    return nullptr; 
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
    EnemyPool.Empty();

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
}
