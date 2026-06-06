// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_Level3.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Enemies/EnemySupport.h"
#include "../../Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"

AGameMode_Level3::AGameMode_Level3()
{
    EnemiesRequired = 8;
    SpawnInterval = 4.f;
    SpawnDistance = 800.f;
    PhaseNumber = 1;
    NextLevelName = FName("Lore_Fase2");
}

void AGameMode_Level3::BeginPlay()
{
    AActor* Found = UGameplayStatics::GetActorOfClass( GetWorld(), APhase1EnemyPool::StaticClass());
    LevelPool = Cast<APhase1EnemyPool>(Found);

    if (LevelPool)
    {
        LevelPool->EnemyClass = AEnemySupport::StaticClass();
        LevelPool->InitializePool();
    }

    Super::BeginPlay();
}

void AGameMode_Level3::SpawnEnemy()
{
    if (!LevelPool) return;

    int32 ActiveCount = (TotalSpawned - EnemiesDefeated);
    if (ActiveCount >= MaxActiveAtOnce) return;

    AEnemyBase* Enemy = LevelPool->GetEnemyFromPool();
    if (!Enemy) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    FVector SpawnLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * SpawnDistance + FVector(0.f, FMath::RandRange(-600.f, 600.f), FMath::RandRange(100.f, 300.f));

    Enemy->SetActorLocation(SpawnLocation);
    Enemy->SetActorHiddenInGame(false);
    Enemy->SetActorEnableCollision(true);
    Enemy->SetActorTickEnabled(true);

    if (GameFacadeInstance)
        GameFacadeInstance->ConfigureEnemy(Enemy, EEnemyType::Support);

    if (AEnemySupport* Support = Cast<AEnemySupport>(Enemy))
        Support->RestartFireTimer();

    TotalSpawned++;
}