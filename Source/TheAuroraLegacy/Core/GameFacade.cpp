  
#include "GameFacade.h"
#include "../AuroraGameInstance.h"
#include "../TheAuroraLegacyGameMode.h"
#include "../Enemies/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGameFacade::AGameFacade()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGameFacade::BeginPlay()
{
    Super::BeginPlay();
}

// ── Helpers privados ─────────────────────────────────────────────────────────

UAuroraGameInstance* AGameFacade::GetGI()
{
    return Cast<UAuroraGameInstance>(GetGameInstance());
}

ATheAuroraLegacyGameMode* AGameFacade::GetGM()
{
    return Cast<ATheAuroraLegacyGameMode>(
        GetWorld()->GetAuthGameMode());
}

// ── Score / Vida / Persistencia ──────────────────────────────────────────────

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
    UE_LOG(LogTemp, Warning, TEXT("Facade: GAME OVER"));
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

// ── Enemigos ─────────────────────────────────────────────────────────────────

void AGameFacade::ConfigureEnemy(AEnemyBase* Enemy, EEnemyType Type)
{
    if (!Enemy) return;

    // Tabla de stats por tipo. Agregar aquí si se suma un nuevo enemigo;
    // ningún GameMode ni enemigo necesita conocer estos valores.
    struct FEnemyStats { int32 Health; float MoveSpeed; int32 ContactDamage; int32 ScoreValue; };

    static const TMap<EEnemyType, FEnemyStats> StatsTable =
    {
        { EEnemyType::Drone,       { 1,  350.f, 1,  100  } },
        { EEnemyType::Hunter,      { 2,  450.f, 1,  200  } },
        { EEnemyType::Support,     { 2,  250.f, 1,  250  } },
        { EEnemyType::Assault,     { 3,  400.f, 1,  300  } },
        { EEnemyType::Maneuver,    { 3,  350.f, 1,  350  } },
        { EEnemyType::Armored,     { 6,  150.f, 2,  500  } },
        { EEnemyType::Interceptor, { 3,  700.f, 2,  600  } },
        { EEnemyType::Turret,      { 6,    0.f, 1,  700  } },
        { EEnemyType::Boss,        { 20,  80.f, 3, 5000  } },
    };

    if (const FEnemyStats* Stats = StatsTable.Find(Type))
    {
        Enemy->Health = Stats->Health;
        Enemy->MoveSpeed = Stats->MoveSpeed;
        Enemy->ContactDamage = Stats->ContactDamage;
        Enemy->ScoreValue = Stats->ScoreValue;
    }
    else
    {
        // Fallback defensivo: stats mínimos
        Enemy->Health = 1;
        Enemy->MoveSpeed = 200.f;
        Enemy->ContactDamage = 1;
        Enemy->ScoreValue = 100;
        UE_LOG(LogTemp, Warning,
            TEXT("Facade: ConfigureEnemy - tipo desconocido %d, usando defaults"),
            (int32)Type);
    }
}

void AGameFacade::NotifyEnemyDefeated(AEnemyBase* Enemy)
{
    if (!Enemy) return;

    // El ScoreValue ya fue configurado por ConfigureEnemy al spawnar,
    // así que lo leemos directamente del enemigo.
    int32 Score = Enemy->ScoreValue;

    // 1. Sumar score en el GameInstance (persiste entre niveles)
    AddScore(Score);

    // 2. Avisar al GameMode para que avance el contador del nivel
    if (ATheAuroraLegacyGameMode* GM = GetGM())
    {
        GM->OnEnemyDefeated(Score);
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Facade: Enemigo derrotado | Score otorgado: %d"), Score);
}
