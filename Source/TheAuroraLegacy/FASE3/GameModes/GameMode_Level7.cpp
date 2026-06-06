// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_Level7.h"
#include "Kismet/GameplayStatics.h"
#include "../../Enemies/EnemyBase.h"

AGameMode_Level7::AGameMode_Level7()
{
    EnemiesRequired = 15;
    SpawnInterval = 15.f;
    SpawnDistance = 5000.f;

    NextLevelName = FName("Level8");
}

void AGameMode_Level7::BeginPlay()
{
    Super::BeginPlay();

	// Spawnea el pool de enemigos
    FActorSpawnParameters SpawnParams;
    EnemyPool = GetWorld()->SpawnActor<APhase3EnemyPool>(
        APhase3EnemyPool::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        SpawnParams);

    if (EnemyPool)
    {
        // Configurar el pool con la clase del enemigo
        EnemyPool->EnemyClass = EnemyClass;
        EnemyPool->PoolSize = 15;
        // Inicializar DESPUES de asignar EnemyClass
        EnemyPool->InitializePool();
        UE_LOG(LogTemp, Warning,
            TEXT("Pool creado desde codigo"));
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Error al crear el Pool"));
    }

    UE_LOG(LogTemp, Warning,
        TEXT("=== NIVEL 7 - INTERCEPTORES ==="));
}

void AGameMode_Level7::SpawnEnemy()
{
    if (EnemiesSpawned >= MaxEnemies)
    {
        GetWorldTimerManager().ClearTimer(
            SpawnTimerHandle);
        return;
    }

    if (!EnemyPool) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(
        GetWorld(), 0);
    if (!PlayerPawn) return;

    FVector PlayerLoc = PlayerPawn->GetActorLocation();

    FVector SpawnLocation = FVector(
        PlayerLoc.X + FMath::RandRange(-800.f, 800.f),
        PlayerLoc.Y + SpawnDistance,
        PlayerLoc.Z
    );

    // Obtener enemigo del pool
    AEnemyBase* Enemy = EnemyPool->GetPooledEnemy();
    if (Enemy)
    {
        Enemy->SetActorLocation(SpawnLocation);
        EnemiesSpawned++;

        UE_LOG(LogTemp, Warning,
            TEXT("Interceptor %d/%d spawneado desde pool"),
            EnemiesSpawned, MaxEnemies);
    }
}