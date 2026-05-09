// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheAuroraLegacyGameMode.h"
#include "TheAuroraLegacyPawn.h"
#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ATheAuroraLegacyGameMode::ATheAuroraLegacyGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    DefaultPawnClass = ATheAuroraLegacyPawn::StaticClass();
   
}

void ATheAuroraLegacyGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Iniciar spawner cada X segundos
    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &ATheAuroraLegacyGameMode::SpawnEnemy,
        SpawnInterval,
        true  // loop
    );
}

void ATheAuroraLegacyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATheAuroraLegacyGameMode::AddScore(int32 Amount)
{
    Score += Amount;
    UE_LOG(LogTemp, Warning, TEXT("Puntuacion: %d"), Score);
}

void ATheAuroraLegacyGameMode::SpawnEnemy()
{
    if (!EnemyClass) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    // Spawner delante del jugador
    FVector SpawnLocation = PlayerPawn->GetActorLocation() +
        PlayerPawn->GetActorForwardVector() * SpawnDistance;
    SpawnLocation.Z = PlayerPawn->GetActorLocation().Z;

    FRotator SpawnRotation = PlayerPawn->GetActorRotation();
    SpawnRotation.Yaw += 180.f;

    GetWorld()->SpawnActor<AEnemyBase>(
        EnemyClass, SpawnLocation, SpawnRotation);

    UE_LOG(LogTemp, Warning, TEXT("Enemigo spawneado!"));
}
