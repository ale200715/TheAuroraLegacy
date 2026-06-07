// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_Level1.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Enemies/EnemyDrone.h"
#include "../../Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"

AGameMode_Level1::AGameMode_Level1()
{
    EnemiesRequired = 15;
    SpawnInterval = 3.f;
    SpawnDistance = 800.f;
    PhaseNumber = 1;
    NextLevelName = FName("Level2_Hunter");
}

void AGameMode_Level1::BeginPlay()
{
    AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), APhase1EnemyPool::StaticClass());
    LevelPool = Cast<APhase1EnemyPool>(Found);

    if (LevelPool)
    {
        LevelPool->EnemyClass = AEnemyDrone::StaticClass();
        LevelPool->InitializePool();
    }

    Super::BeginPlay(); 
}

void AGameMode_Level1::SpawnEnemy()
{
    if (!LevelPool) return;

    // Respetar el límite de activos simultáneos
    int32 ActiveCount = (TotalSpawned - EnemiesDefeated);
    if (ActiveCount >= MaxActiveAtOnce) return;

    AEnemyBase* Enemy = LevelPool->GetEnemyFromPool();
    if (!Enemy) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    FVector SpawnLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * SpawnDistance + FVector(0.f, FMath::RandRange(-200.f, 200.f),FMath::RandRange(0.f, 100.f));

    Enemy->SetActorLocation(SpawnLocation);
    Enemy->SetActorHiddenInGame(false);
    Enemy->SetActorEnableCollision(true);
    Enemy->SetActorTickEnabled(true);

    // El Facade configura los stats — no los seteamos a mano
    if (GameFacadeInstance)
        GameFacadeInstance->ConfigureEnemy(Enemy, EEnemyType::Drone);

    if (AEnemyDrone* Drone = Cast<AEnemyDrone>(Enemy))
        Drone->RestartFireTimer();

    TotalSpawned++;
}