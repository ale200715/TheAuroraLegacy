// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Level3.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Enemies/EnemySupport.h"
#include "../Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"

AGameMode_Level3::AGameMode_Level3()
{
    EnemiesRequired = 8;
    SpawnInterval = 4.f;
    SpawnDistance = 800.f;

    // Después del nivel 3 va el lore de Fase 2
    NextLevelName = FName("Lore_Fase2");
}

void AGameMode_Level3::BeginPlay()
{
    FindPool();

    // Asignar clase Y inicializar el pool
    // directamente desde el GameMode
    if (Level3Pool)
    {
        Level3Pool->EnemyClass =
            AEnemySupport::StaticClass();

        // Llamar inicialización manualmente
        Level3Pool->InitializePool();

        UE_LOG(LogTemp, Warning,
            TEXT("Level3: Pool inicializado "
                "con Supports"));
    }

    // Registrar en Facade
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
                EEnemyType::Support,
                AEnemySupport::StaticClass());
        }
    }

    Super::BeginPlay();

    UE_LOG(LogTemp, Warning,
        TEXT("Level3: Iniciado. "
            "Derrotar %d supports para pasar"),
        EnemiesRequired);
}

void AGameMode_Level3::SpawnEnemy()
{
    if (EnemiesDefeated >= EnemiesRequired)
    {
        GetWorldTimerManager().ClearTimer(
            SpawnTimerHandle);
        return;
    }

    if (TotalSpawned - EnemiesDefeated >=
        MaxActiveAtOnce)
        return;

    if (!Level3Pool)
    {
        FindPool();
        if (!Level3Pool) return;
    }

    AEnemyBase* Support =
        Level3Pool->GetEnemyFromPool();

    if (!Support)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Level3: Pool sin supports"));
        return;
    }

    APawn* Player =
        UGameplayStatics::GetPlayerPawn(
            GetWorld(), 0);

    if (!Player) return;

    // El Support aparece en los bordes
    // ya que su movimiento es horizontal
    float RandomY = FMath::RandRange(
        -600.f, 600.f);
    float RandomZ = FMath::RandRange(
        100.f, 300.f);

    FVector SpawnLocation =
        Player->GetActorLocation() +
        Player->GetActorForwardVector() *
        SpawnDistance;

    SpawnLocation.Y = RandomY;
    SpawnLocation.Z += RandomZ;

    Support->SetActorLocation(SpawnLocation);
    Support->SetActorHiddenInGame(false);
    Support->SetActorEnableCollision(true);
    Support->SetActorTickEnabled(true);
    Support->Health = 2;

    // Reiniciar timer de disparo
    AEnemySupport* SupportCast =
        Cast<AEnemySupport>(Support);
    if (SupportCast)
        SupportCast->RestartFireTimer();

    TotalSpawned++;

    UE_LOG(LogTemp, Warning,
        TEXT("Level3: Support %d spawneado. "
            "Derrotados: %d/%d"),
        TotalSpawned,
        EnemiesDefeated,
        EnemiesRequired);
}

void AGameMode_Level3::FindPool()
{
    AActor* FoundActor =
        UGameplayStatics::GetActorOfClass(
            GetWorld(),
            APhase1EnemyPool::StaticClass());

    Level3Pool = Cast<APhase1EnemyPool>(FoundActor);

    if (Level3Pool) {

        UE_LOG(LogTemp, Warning,
            TEXT("Level3: Pool encontrado"));
    }
    else {

        UE_LOG(LogTemp, Error,
            TEXT("Level3: No se encontro el Pool"));
    }
}