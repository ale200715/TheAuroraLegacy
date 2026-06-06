// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_Level2.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Enemies/EnemyHunter.h"
#include "../../Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"

AGameMode_Level2::AGameMode_Level2()
{
    EnemiesRequired = 10;
    SpawnInterval = 3.f;
    SpawnDistance = 800.f;
    PhaseNumber = 1;
    NextLevelName = FName("Level3_Support");
}

void AGameMode_Level2::BeginPlay()
{
    AActor* Found = UGameplayStatics::GetActorOfClass(
        GetWorld(), APhase1EnemyPool::StaticClass());
    LevelPool = Cast<APhase1EnemyPool>(Found);

    if (LevelPool)
    {
        LevelPool->EnemyClass = AEnemyHunter::StaticClass();
        LevelPool->InitializePool();
    }

    Super::BeginPlay();
}

void AGameMode_Level2::SpawnEnemy()
{
    if (!LevelPool) return;

    int32 ActiveCount = (TotalSpawned - EnemiesDefeated);
    if (ActiveCount >= MaxActiveAtOnce) return;

    AEnemyBase* Enemy = LevelPool->GetEnemyFromPool();
    if (!Enemy) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    FVector SpawnLocation = Player->GetActorLocation()+ Player->GetActorForwardVector() * SpawnDistance + FVector(0.f, FMath::RandRange(-300.f, 300.f), FMath::RandRange(0.f, 100.f));

    Enemy->SetActorLocation(SpawnLocation);
    Enemy->SetActorHiddenInGame(false);
    Enemy->SetActorEnableCollision(true);
    Enemy->SetActorTickEnabled(true);

    if (GameFacadeInstance)
        GameFacadeInstance->ConfigureEnemy(Enemy, EEnemyType::Hunter);

    if (AEnemyHunter* Hunter = Cast<AEnemyHunter>(Enemy))
        Hunter->RestartFireTimer();

    TotalSpawned++;
}