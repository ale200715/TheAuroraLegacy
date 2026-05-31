// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Level9.h"
#include "Kismet/GameplayStatics.h"
#include "../../Enemies/EnemyBase.h"

AGameMode_Level9::AGameMode_Level9()
{
    // Solo 1 enemigo: la Nave Nodriza
    EnemiesRequired = 1;
    // Aparece a los 3 segundos de iniciar
    SpawnInterval = 3.f;
    SpawnDistance = 3000.f;
}

void AGameMode_Level9::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning,
        TEXT("=== NIVEL 9 - NAVE NODRIZA ==="));
}

void AGameMode_Level9::SpawnEnemy()
{
    // Solo spawnear una vez
    if (bBossSpawned) return;

    if (!EnemyClass) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(
        GetWorld(), 0);
    if (!PlayerPawn) return;

    FVector PlayerLoc = PlayerPawn->GetActorLocation();

    // Aparecer adelante y centrado
    FVector SpawnLocation = FVector(
        PlayerLoc.X,
        PlayerLoc.Y + SpawnDistance,
        PlayerLoc.Z
    );

    GetWorld()->SpawnActor<AEnemyBase>(
        EnemyClass, SpawnLocation,
        FRotator::ZeroRotator);

    bBossSpawned = true;

    // Detener el timer, no necesitamos mas spawns
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

    UE_LOG(LogTemp, Warning,
        TEXT("Nave Nodriza spawneada"));
}

