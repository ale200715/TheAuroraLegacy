#include "GameMode_Level5.h"
#include "../Enemies/TankEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../Core/GameFacade.h"
#include "../../Enemies/EnemyBase.h" 

AGameMode_Level5::AGameMode_Level5()
{
    PrimaryActorTick.bCanEverTick = false;
    EnemiesRequired = 8;
    NextLevelName = FName("Level6");
    PhaseNumber = 2;
}

void AGameMode_Level5::SpawnEnemy()
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

    ATankEnemy* Enemy = GetWorld()->SpawnActor<ATankEnemy>(
        EnemyClass, SpawnLocation, SpawnRotation);

    if (Enemy && GameFacadeInstance)
        GameFacadeInstance->ConfigureEnemy(Enemy, EEnemyType::Armored);
}

