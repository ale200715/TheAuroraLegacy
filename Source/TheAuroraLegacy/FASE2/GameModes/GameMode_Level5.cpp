#include "GameMode_Level5.h"
#include "../Enemies/TankEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGameMode_Level5::AGameMode_Level5()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGameMode_Level5::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AGameMode_Level5::SpawnTank,
        6.f,
        true,
        2.f
    );
}

void AGameMode_Level5::SpawnTank()
{
    if (!EnemyClass) return;
    if (EnemiesDefeated >= EnemiesToDefeat) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    FVector SpawnLocation = Player->GetActorLocation() +
        Player->GetActorForwardVector() * 2000.f;
    SpawnLocation.Z = Player->GetActorLocation().Z;

    FRotator SpawnRotation = Player->GetActorRotation();
    SpawnRotation.Yaw += 180.f;

    GetWorld()->SpawnActor<ATankEnemy>(
        EnemyClass, SpawnLocation, SpawnRotation);

    OnTankGroupSpawned.Broadcast(1);

    int32 Remaining = EnemiesToDefeat - EnemiesDefeated;
    OnTankCountChanged.Broadcast(Remaining);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
            FString::Printf(TEXT("¡Tank spawneado! Faltan derrotar: %d"), Remaining));
    }

    UE_LOG(LogTemp, Warning, TEXT("Tank spawneado!"));
}

void AGameMode_Level5::OnEnemyDefeated()
{
    EnemiesDefeated++;

    int32 Remaining = EnemiesToDefeat - EnemiesDefeated;
    OnTankCountChanged.Broadcast(Remaining);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
            FString::Printf(TEXT("Tank destruido! Faltan: %d/%d"),
                Remaining, EnemiesToDefeat));
    }

    if (EnemiesDefeated >= EnemiesToDefeat)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("¡NIVEL 5 COMPLETADO!"));
        }
        UE_LOG(LogTemp, Warning, TEXT("NIVEL 5 COMPLETADO!"));
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }
}

