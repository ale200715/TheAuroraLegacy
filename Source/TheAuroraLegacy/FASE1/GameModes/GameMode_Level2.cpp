// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Level2.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Enemies/EnemyHunter.h"
#include "../Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"

AGameMode_Level2::AGameMode_Level2()
{
    EnemiesRequired = 10;
    SpawnInterval = 3.f;
    SpawnDistance = 800.f;
    NextLevelName = FName("Level3_Support");
}

void AGameMode_Level2::BeginPlay()
{
    FindPool();

    if (Level2Pool)
    {
        Level2Pool->EnemyClass = AEnemyHunter::StaticClass(); 
        Level2Pool->InitializePool();
    }

    TArray<AActor*> FoundFacades;
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), AGameFacade::StaticClass(), FoundFacades);

    if (FoundFacades.Num() > 0)
    {
        AGameFacade* Facade = Cast<AGameFacade>(FoundFacades[0]);
        if (Facade)
        {
            Facade->RegisterEnemyClass( EEnemyType::Hunter, AEnemyHunter::StaticClass());

        }
    }

    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("Level2: Iniciado. Derrotar %d hunters para pasar"), EnemiesRequired);
}

void AGameMode_Level2::SpawnEnemy()
{
    if (EnemiesDefeated >= EnemiesRequired)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    if (TotalSpawned - EnemiesDefeated >= MaxActiveAtOnce) {
        return;
    }

    if (!Level2Pool)
    {
        FindPool();
        if (!Level2Pool) return;
    }

    AEnemyBase* Hunter =Level2Pool->GetEnemyFromPool();


    APawn* Player = UGameplayStatics::GetPlayerPawn( GetWorld(), 0);

    if (!Player) return;

    float RandomY = FMath::RandRange( -300.f, 300.f);
    float RandomZ = FMath::RandRange( 0.f, 100.f);

    FVector SpawnLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * SpawnDistance;

    SpawnLocation.Y += RandomY;
    SpawnLocation.Z += RandomZ;

    Hunter->SetActorLocation(SpawnLocation);
    Hunter->SetActorHiddenInGame(false);
    Hunter->SetActorEnableCollision(true);
    Hunter->SetActorTickEnabled(true);
    Hunter->Health = 2;

    AEnemyHunter* HunterCast = Cast<AEnemyHunter>(Hunter);
    if (HunterCast)
    {
        HunterCast->RestartFireTimer();
    }

    TotalSpawned++;

    UE_LOG(LogTemp, Warning, TEXT("Level2: Hunter %d spawneado. Derrotados: %d/%d"),TotalSpawned,EnemiesDefeated, EnemiesRequired);
}

void AGameMode_Level2::FindPool()
{
    AActor* FoundActor =UGameplayStatics::GetActorOfClass(GetWorld(), APhase1EnemyPool::StaticClass());

    Level2Pool = Cast<APhase1EnemyPool>(FoundActor);

}
