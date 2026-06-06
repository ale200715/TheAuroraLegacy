#include "GameMode_Level6.h"
#include "../Enemies/DestroyerEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../Core/GameFacade.h"
#include "../../Enemies/EnemyBase.h"

AGameMode_Level6::AGameMode_Level6()
{
    PrimaryActorTick.bCanEverTick = false;
    EnemiesRequired = 5;
    NextLevelName = FName("Lore_Fase3");
    PhaseNumber = 2;
}

void AGameMode_Level6::SpawnEnemy()
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

    ADestroyerEnemy* Enemy = GetWorld()->SpawnActor<ADestroyerEnemy>(
        EnemyClass, SpawnLocation, SpawnRotation);

    if (Enemy && GameFacadeInstance)
        GameFacadeInstance->ConfigureEnemy(Enemy, EEnemyType::Maneuver);
}