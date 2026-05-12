#include "EnemyBase.h"
#include "../Core/GameFacade.h"
#include "../TheAuroraLegacyGameMode.h"
#include "Kismet/GameplayStatics.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorEnableCollision(true);
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Llamar al movimiento virtual
    // Cada enemigo sobreescribe MoveEnemy
    // con su propio comportamiento
    if (IsAlive())
    {
        MoveEnemy(DeltaTime);
    }
}

void AEnemyBase::MoveEnemy(float DeltaTime)
{
    // La base no hace nada
    // Cada enemigo sobreescribe esto
}

void AEnemyBase::TakeDamageEnemy(
    int32 DamageAmount)
{
    Health -= DamageAmount;

    UE_LOG(LogTemp, Warning,
        TEXT("Enemigo recibio daño. "
            "Vida restante: %d"), Health);

    if (Health <= 0)
    {
        OnDeath();
    }
}

void AEnemyBase::OnDeath()
{
    UE_LOG(LogTemp, Warning,
        TEXT("Enemigo muerto. Puntos: %d"),
        ScoreValue);

    // 1. Notificar al GameFacade
    TArray<AActor*> FoundFacades;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AGameFacade::StaticClass(),
        FoundFacades);

    if (FoundFacades.Num() > 0)
    {
        AGameFacade* Facade =
            Cast<AGameFacade>(FoundFacades[0]);
        if (Facade)
        {
            Facade->NotifyEnemyDefeated(this);
        }
    }

    // 2. Notificar al GameMode
    ATheAuroraLegacyGameMode* GM =
        Cast<ATheAuroraLegacyGameMode>(
            GetWorld()->GetAuthGameMode());
    if (GM)
    {
        GM->OnEnemyDefeated(ScoreValue);
    }

    // 3. Destruirse
    Destroy();
}
