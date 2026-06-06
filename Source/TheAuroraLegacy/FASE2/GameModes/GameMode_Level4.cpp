#include "GameMode_Level4.h"
#include "../Enemies/InfantryEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../Core/GameFacade.h"
#include "../../Enemies/EnemyBase.h" 

AGameMode_Level4::AGameMode_Level4()
{
    PrimaryActorTick.bCanEverTick = false;
    EnemiesRequired = 10;
    NextLevelName = FName("Level5");
    PhaseNumber = 2;
}

void AGameMode_Level4::SpawnEnemy()
{
    if (!EnemyClass) return;
    if (EnemiesDefeated >= EnemiesRequired) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    for (int32 i = 0; i < 3; i++)
    {
        FVector SpawnLocation = Player->GetActorLocation() +
            Player->GetActorForwardVector() * 2000.f +
            FVector(0.f, i * 200.f - 200.f, 0.f);
        FRotator SpawnRotation = Player->GetActorRotation();
        SpawnRotation.Yaw += 180.f;

        AInfantryEnemy* Enemy = GetWorld()->SpawnActor<AInfantryEnemy>(
            EnemyClass, SpawnLocation, SpawnRotation);

        if (Enemy && GameFacadeInstance)
            GameFacadeInstance->ConfigureEnemy(Enemy, EEnemyType::Assault);
    }
}