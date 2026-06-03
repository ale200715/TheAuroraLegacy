// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_Level8.h"
#include "Kismet/GameplayStatics.h"
#include "../../Enemies/EnemyBase.h"

AGameMode_Level8::AGameMode_Level8()
{
    // Segun documento: 5 torretas
    EnemiesRequired = 5;
    // Aparece una torreta cada 20 segundos
    SpawnInterval = 20.f;
    SpawnDistance = 3000.f;

    // Posiciones relativas al jugador
    // X = izquierda/derecha
    // Y = distancia adelante
    // Z = altura
    SpawnPositions.Add(FVector(-600.f, 3000.f, 0.f));
    SpawnPositions.Add(FVector(0.f, 3500.f, 0.f));
    SpawnPositions.Add(FVector(600.f, 3000.f, 0.f));
    SpawnPositions.Add(FVector(-400.f, 4000.f, 0.f));
    SpawnPositions.Add(FVector(400.f, 4000.f, 0.f));

	NextLevelName = FName("Level9");
}

void AGameMode_Level8::BeginPlay()
{
    Super::BeginPlay();

    FActorSpawnParameters SpawnParams;
    EnemyPool = GetWorld()->SpawnActor<APhase3EnemyPool>(
        APhase3EnemyPool::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        SpawnParams);

    if (EnemyPool)
    {
        EnemyPool->EnemyClass = EnemyClass;
        EnemyPool->PoolSize = 5;
        EnemyPool->InitializePool();
        UE_LOG(LogTemp, Warning,
            TEXT("Pool Nivel 8 creado"));
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Error al crear Pool Nivel 8"));
    }

    UE_LOG(LogTemp, Warning,
        TEXT("=== NIVEL 8 - TORRETAS ==="));
}

void AGameMode_Level8::SpawnEnemy()
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

    // Usar posicion fija segun el numero de torreta
    FVector Offset = SpawnPositions[EnemiesSpawned];
    FVector SpawnLocation = FVector(
        PlayerLoc.X + Offset.X,
        PlayerLoc.Y + Offset.Y,
        PlayerLoc.Z + Offset.Z
    );

    AEnemyBase* Enemy = EnemyPool->GetPooledEnemy();
    if (Enemy)
    {
        Enemy->SetActorLocation(SpawnLocation);
        EnemiesSpawned++;

        UE_LOG(LogTemp, Warning,
            TEXT("Torreta %d/%d en posicion fija"),
            EnemiesSpawned, MaxEnemies);
    }
}
