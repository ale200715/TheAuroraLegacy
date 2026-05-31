#include "Phase1EnemyPool.h"
#include "../Enemies/EnemyDrone.h"     
#include "../Projectiles/EnemyProjectile.h"

APhase1EnemyPool::APhase1EnemyPool()
{
    PrimaryActorTick.bCanEverTick = false;

    ProjectileClass = AEnemyProjectile::StaticClass();
    PoolSize = 20;
}

void APhase1EnemyPool::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle InitTimer;
    GetWorldTimerManager().SetTimer( InitTimer, this, &APhase1EnemyPool::InitializePool, 0.1f, false);

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

    for (int32 i = 0; i < 50; i++)
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
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error,TEXT("Pool: No hay EnemyClass asignada"));
        return;
    }

    EnemyPool.Empty();

    for (int32 i = 0; i < PoolSize; i++)
    {
        AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass,  FVector::ZeroVector, FRotator::ZeroRotator);

        if (NewEnemy)
        {
            NewEnemy->SetActorHiddenInGame(true);
            NewEnemy->SetActorTickEnabled(false);
            NewEnemy->SetActorEnableCollision(false);
            EnemyPool.Add(NewEnemy);
        }
    }

    UE_LOG(LogTemp, Warning,TEXT("Pool: Inicializado con %d enemigos  de clase %s"), EnemyPool.Num(), *EnemyClass->GetName());
}

void APhase1EnemyPool::ReinitializePool(TSubclassOf<AEnemyBase> NewEnemyClass)
{
    for (AEnemyBase* Enemy : EnemyPool)
    {
        if (Enemy) Enemy->Destroy();
    }
    EnemyPool.Empty();

    EnemyClass = NewEnemyClass;
    for (int32 i = 0; i < PoolSize; i++)
    {
        if (!EnemyClass) continue;

        AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>( EnemyClass, FVector::ZeroVector, FRotator::ZeroRotator);

        if (NewEnemy)
        {
            NewEnemy->SetActorHiddenInGame(true);
            NewEnemy->SetActorTickEnabled(false);
            NewEnemy->SetActorEnableCollision(false);
            EnemyPool.Add(NewEnemy);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Pool: Reinicializado con %d enemigos"), EnemyPool.Num());
}