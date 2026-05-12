#include "GameFacade.h"
#include "../AuroraGameInstance.h"
#include "../TheAuroraLegacyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Enemies/EnemyBase.h"
#include "Engine/World.h"

AGameFacade::AGameFacade()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGameFacade::BeginPlay()
{
    Super::BeginPlay();
}

UAuroraGameInstance* AGameFacade::GetGI()
{
    return Cast<UAuroraGameInstance>(
        GetGameInstance());
}

ATheAuroraLegacyGameMode* AGameFacade::GetGM()
{
    return Cast<ATheAuroraLegacyGameMode>(
        GetWorld()->GetAuthGameMode());
}

void AGameFacade::AddScore(int32 Amount)
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->AddScore(Amount);
}

void AGameFacade::LoseLife()
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->LoseLife();
}

void AGameFacade::SpawnEnemy()
{
    if (ATheAuroraLegacyGameMode* GM = GetGM())
        GM->SpawnEnemy();
}

void AGameFacade::SaveGame()
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->SaveGame();
}

void AGameFacade::LoadGame()
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->LoadGame();
}

void AGameFacade::TriggerGameOver()
{
    SaveGame();
    UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void AGameFacade::RegisterEnemyClass(
    EEnemyType Type,
    TSubclassOf<AEnemyBase> EnemyClass)
{
    EnemyClasses.Add(Type, EnemyClass);
    UE_LOG(LogTemp, Warning,
        TEXT("Facade: Clase registrada tipo %d"),
        (int32)Type);
}


void AGameFacade::SpawnWave(
    EEnemyType Type,
    int32 Count,
    FVector CenterLocation)
{
    for (int32 i = 0; i < Count; i++)
    {
        float Offset = (i - Count / 2) * 200.f;
        FVector SpawnLocation = CenterLocation;
        SpawnLocation.Y += Offset;

        // Buscar clase registrada
        TSubclassOf<AEnemyBase>* FoundClass =
            EnemyClasses.Find(Type);

        if (!FoundClass || !(*FoundClass))
        {
            UE_LOG(LogTemp, Error,
                TEXT("Facade: No hay clase "
                    "para tipo %d"),
                (int32)Type);
            continue;
        }

        // Spawnear
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod
            ::AdjustIfPossibleButAlwaysSpawn;

        AEnemyBase* NewEnemy =
            GetWorld()->SpawnActor<AEnemyBase>(
                *FoundClass,
                SpawnLocation,
                FRotator::ZeroRotator,
                SpawnParams);

        if (NewEnemy)
        {
            ConfigureEnemy(NewEnemy, Type);
            ActiveEnemies.Add(NewEnemy);
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Facade: Oleada de %d spawneada"),
        Count);
}

void AGameFacade::NotifyEnemyDefeated(
    AEnemyBase* Enemy)
{
    ActiveEnemies.Remove(Enemy);
    DefeatedCount++;
    UE_LOG(LogTemp, Warning,
        TEXT("Facade: Derrotados: %d"),
        DefeatedCount);
}

void AGameFacade::ClearAllEnemies()
{
    for (AEnemyBase* Enemy : ActiveEnemies)
        if (Enemy && !Enemy->IsPendingKill())
            Enemy->Destroy();
    ActiveEnemies.Empty();
    DefeatedCount = 0;
}

int32 AGameFacade::GetDefeatedCount() const
{
    return DefeatedCount;
}

bool AGameFacade::HasActiveEnemies() const
{
    for (AEnemyBase* Enemy : ActiveEnemies)
        if (Enemy && !Enemy->IsPendingKill())
            return true;
    return false;
}

void AGameFacade::ConfigureEnemy(
    AEnemyBase* Enemy,
    EEnemyType Type)
{
    if (!Enemy) return;

    switch (Type)
    {
    case EEnemyType::Drone:
        Enemy->Health = 1;
        Enemy->MoveSpeed = 350.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 100;
        break;
    case EEnemyType::Hunter:
        Enemy->Health = 2;
        Enemy->MoveSpeed = 450.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 200;
        break;
    case EEnemyType::Support:
        Enemy->Health = 2;
        Enemy->MoveSpeed = 250.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 250;
        break;
    case EEnemyType::Assault:
        Enemy->Health = 3;
        Enemy->MoveSpeed = 400.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 300;
        break;
    case EEnemyType::Maneuver:
        Enemy->Health = 3;
        Enemy->MoveSpeed = 350.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 350;
        break;
    case EEnemyType::Armored:
        Enemy->Health = 6;
        Enemy->MoveSpeed = 150.f;
        Enemy->ContactDamage = 2;
        Enemy->ScoreValue = 500;
        break;
    case EEnemyType::Interceptor:
        Enemy->Health = 3;
        Enemy->MoveSpeed = 700.f;
        Enemy->ContactDamage = 2;
        Enemy->ScoreValue = 600;
        break;
    case EEnemyType::Turret:
        Enemy->Health = 6;
        Enemy->MoveSpeed = 0.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 700;
        break;
    case EEnemyType::Boss:
        Enemy->Health = 20;
        Enemy->MoveSpeed = 80.f;
        Enemy->ContactDamage = 3;
        Enemy->ScoreValue = 5000;
        break;
    default:
        Enemy->Health = 1;
        Enemy->MoveSpeed = 200.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 100;
        break;
    }
}