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

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &ATheAuroraLegacyGameMode::SpawnEnemy,
        SpawnInterval,
        true  
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

void ATheAuroraLegacyGameMode::OnEnemyDefeated(
    int32 ScoreValue)
{ 
   
    EnemiesDefeated++;
    AddScore(ScoreValue);

    UE_LOG(LogTemp, Warning,
        TEXT("Enemigos: %d / %d"),
        EnemiesDefeated, EnemiesRequired);

    CheckLevelComplete();
  
}
void ATheAuroraLegacyGameMode::CheckLevelComplete()
{
    if (EnemiesDefeated >= EnemiesRequired)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Nivel completado!"));

        // Detener el spawner
        GetWorldTimerManager().ClearTimer(
            SpawnTimerHandle);

        // Esperar 2 segundos y cargar siguiente
        FTimerHandle CompleteTimer;
        GetWorldTimerManager().SetTimer(
            CompleteTimer,
            this,
            &ATheAuroraLegacyGameMode::LoadNextLevel,
            2.f,
            false);
    }
}

void ATheAuroraLegacyGameMode::LoadNextLevel()
{
    if (NextLevelName != NAME_None)
    {
        UGameplayStatics::OpenLevel(
            this, NextLevelName);
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("No hay siguiente nivel definido"));
    }
}

void ATheAuroraLegacyGameMode::OnPlayerDeath()
{
    GetWorldTimerManager().ClearTimer(
        SpawnTimerHandle);
    ShowGameOver();
}

void ATheAuroraLegacyGameMode::ShowGameOver()
{
    if (!GameOverWidgetClass) return;

    UGameOverWidget* GOWidget =
        CreateWidget<UGameOverWidget>(
            GetWorld(), GameOverWidgetClass);

    if (GOWidget)
    {
        GOWidget->SetupGameOver(
            1, 1,
            FName(*GetWorld()->GetMapName()));
        GOWidget->AddToViewport();

        APlayerController* PC =
            UGameplayStatics::GetPlayerController(
                this, 0);
        if (PC)
        {
            PC->SetShowMouseCursor(true);
            PC->SetInputMode(FInputModeUIOnly());
        }
    }
}