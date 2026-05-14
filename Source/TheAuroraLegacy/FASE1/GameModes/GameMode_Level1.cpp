// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Level1.h"

#include "../Pool/Phase1EnemyPool.h"
#include "../Enemies/EnemyDrone.h"
#include "../Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"

AGameMode_Level1::AGameMode_Level1()
{
    // Configuración del nivel 1
   EnemiesRequired = 15;
    SpawnInterval = 3.f;
    SpawnDistance = 800.f;

    // Siguiente nivel al terminar
    NextLevelName = FName("Level2_Hunter");
}

void AGameMode_Level1::BeginPlay()
{
    // Buscar el pool antes de llamar al padre
    FindPool();

    // Registrar el Drone en el Facade
    TArray<AActor*> FoundFacades;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AGameFacade::StaticClass(),
        FoundFacades);

    if (FoundFacades.Num() > 0)
    {
        AGameFacade* Facade =
            Cast<AGameFacade>(FoundFacades[0]);
        if (Facade)
        {
            Facade->RegisterEnemyClass(
                EEnemyType::Drone,
                AEnemyDrone::StaticClass());

            UE_LOG(LogTemp, Warning,
                TEXT("Level1: Drone registrado "
                    "en el Facade"));
        }
    }

    // Llamar al padre que inicia el spawner
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning,
        TEXT("Level1: Iniciado. "
            "Derrotar %d drones para pasar"),
        EnemiesRequired);
}

void AGameMode_Level1::SpawnEnemy()
{
    // No spawnear más si ya se completó el nivel
    if (EnemiesDefeated >= EnemiesRequired)
    {
        GetWorldTimerManager().ClearTimer(
            SpawnTimerHandle);
        return;
    }

    // No spawnear si hay demasiados activos
    if (TotalSpawned - EnemiesDefeated >=
        MaxActiveAtOnce)
    {
        return;
    }

    if (!Level1Pool)
    {
        FindPool();
        if (!Level1Pool) return;
    }

    // Obtener drone del pool
    AEnemyBase* Drone =
        Level1Pool->GetEnemyFromPool();

    if (!Drone)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Level1: Pool sin drones "
                "disponibles"));
        return;
    }

    // Posicionar adelante del jugador
    APawn* Player =
        UGameplayStatics::GetPlayerPawn(
            GetWorld(), 0);

    if (!Player) return;

    // Posición aleatoria alrededor del jugador
    float RandomY = FMath::RandRange(
        -200.f, 200.f);
    float RandomZ = FMath::RandRange(
        0.f, 100.f);

    FVector SpawnLocation =
        Player->GetActorLocation() +
        Player->GetActorForwardVector() *
        SpawnDistance;

    SpawnLocation.Y += RandomY;
    SpawnLocation.Z += RandomZ;

    // Activar el drone desde el pool
    Drone->SetActorLocation(SpawnLocation);
    Drone->SetActorHiddenInGame(false);
    Drone->SetActorEnableCollision(true);
    Drone->SetActorTickEnabled(true);
    Drone->Health = 1; // Restaurar vida

    TotalSpawned++;

    UE_LOG(LogTemp, Warning,
        TEXT("Level1: Drone %d spawneado. "
            "Derrotados: %d/%d"),
        TotalSpawned,
        EnemiesDefeated,
        EnemiesRequired);
}

void AGameMode_Level1::FindPool()
{
    AActor* FoundActor =
        UGameplayStatics::GetActorOfClass(
            GetWorld(),
            APhase1EnemyPool::StaticClass());

    Level1Pool = Cast<APhase1EnemyPool>(
        FoundActor);

    if (Level1Pool)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Level1: Pool encontrado"));
    }
    else
    {
        UE_LOG(LogTemp, Error,
            TEXT("Level1: No se encontro "
                "el Pool en el nivel"));
    }
}
