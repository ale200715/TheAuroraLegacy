#include "GameMode_Level4.h"
#include "../Enemies/InfantryEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGameMode_Level4::AGameMode_Level4()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGameMode_Level4::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AGameMode_Level4::SpawnEnemyGroup,
        8.f,
        true,
        2.f
    );
}

void AGameMode_Level4::SpawnEnemyGroup()
{
    if (!EnemyClass) return;
    if (EnemiesDefeated >= EnemiesToDefeat) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    for (int32 i = 0; i < 3; i++)
    {
        FVector SpawnLocation = Player->GetActorLocation() +
            Player->GetActorForwardVector() * 2000.f +
            FVector(0.f, i * 200.f - 200.f, 0.f);

        FRotator SpawnRotation = Player->GetActorRotation();
        SpawnRotation.Yaw += 180.f;

        GetWorld()->SpawnActor<AInfantryEnemy>(
            EnemyClass, SpawnLocation, SpawnRotation);

        ActiveEnemies++;
    }

    // Observer notifica
    OnEnemyGroupSpawned.Broadcast(3);

    int32 Remaining = EnemiesToDefeat - EnemiesDefeated;
    OnEnemyCountChanged.Broadcast(Remaining);

    // Mostrar en pantalla
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
            FString::Printf(TEXT("¡Nuevo grupo de enemigos! Faltan derrotar: %d"), Remaining));
    }
}

void AGameMode_Level4::OnEnemyDefeated()
{
    EnemiesDefeated++;
    ActiveEnemies--;

    int32 Remaining = EnemiesToDefeat - EnemiesDefeated;

    // Observer notifica
    OnEnemyCountChanged.Broadcast(Remaining);

    // Mostrar en pantalla
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
            FString::Printf(TEXT("Enemigo destruido! Faltan: %d/%d"),
                Remaining, EnemiesToDefeat));
    }

    if (EnemiesDefeated >= EnemiesToDefeat)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("¡NIVEL 4 COMPLETADO!"));
        }
        UE_LOG(LogTemp, Warning, TEXT("NIVEL 4 COMPLETADO!"));
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    }
}