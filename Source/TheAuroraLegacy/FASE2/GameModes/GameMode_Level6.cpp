#include "GameMode_Level6.h"
#include "../Enemies/DestroyerEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGameMode_Level6::AGameMode_Level6()
{
    PrimaryActorTick.bCanEverTick = false;

    EnemiesRequired = 5;
    NextLevelName = FName("Lore_Fase3");
}

void AGameMode_Level6::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AGameMode_Level6::SpawnDestroyer,
        10.f,
        true,
        2.f
    );
}

void AGameMode_Level6::SpawnDestroyer()
{
    if (!EnemyClass) return;
    if (EnemiesDefeated >= EnemiesRequired) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    FVector SpawnLocation = Player->GetActorLocation() +
        Player->GetActorForwardVector() * 2000.f;
    SpawnLocation.Z = Player->GetActorLocation().Z;

    FRotator SpawnRotation = Player->GetActorRotation();
    SpawnRotation.Yaw += 180.f;

    GetWorld()->SpawnActor<ADestroyerEnemy>(
        EnemyClass, SpawnLocation, SpawnRotation);

    int32 Remaining = EnemiesRequired - EnemiesDefeated;
    OnDestroyerCountChanged.Broadcast(Remaining);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple,
            FString::Printf(TEXT("Destroyer spawneado! Faltan: %d"), Remaining));
    }

    UE_LOG(LogTemp, Warning, TEXT("Destroyer spawneado!"));
}

void AGameMode_Level6::OnEnemyDefeated()
{
    EnemiesDefeated++;

    int32 Remaining = EnemiesRequired - EnemiesDefeated;
    OnDestroyerCountChanged.Broadcast(Remaining);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
            FString::Printf(TEXT("Destroyer destruido! Faltan: %d/%d"),
                Remaining, EnemiesRequired));
    }

    if (EnemiesDefeated >= EnemiesRequired)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("NIVEL 6 COMPLETADO!"));
        }
        UE_LOG(LogTemp, Warning, TEXT("NIVEL 6 COMPLETADO!"));
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

        LoadNextLevel(); 
    }
}