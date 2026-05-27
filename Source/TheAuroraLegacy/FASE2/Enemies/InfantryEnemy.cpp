#include "InfantryEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "../GameModes/GameMode_Level4.h"
#include "../AuroraGameInstance.h"

AInfantryEnemy::AInfantryEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    Health = 3;
    MoveSpeed = 600.f;
}

void AInfantryEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Suscribirse al evento de destrucción
    OnDestroyed.AddDynamic(this, &AInfantryEnemy::OnInfantryDestroyed);

    GetWorldTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AInfantryEnemy::FireBurst,
        0.5f,
        true
    );
}

void AInfantryEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AInfantryEnemy::FireBurst()
{
    if (!IsValid(this)) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("Infantry disparo al jugador!"));
    }
}

void AInfantryEnemy::NotifyDeath()
{
    UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(
        GetGameInstance());
    if (GI)
    {
        GI->AddScore(100);
        UE_LOG(LogTemp, Warning, TEXT("Infantry destruido! +100 puntos"));
    }

    // Observer: notificar al GameMode
    AGameMode_Level4* GM = Cast<AGameMode_Level4>(
        GetWorld()->GetAuthGameMode());
    if (GM)
    {
        GM->OnEnemyDefeated();
        UE_LOG(LogTemp, Warning, TEXT("GameMode notificado!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameMode no encontrado!"));
    }
}

void AInfantryEnemy::OnInfantryDestroyed(AActor* DestroyedActor)
{
    NotifyDeath();
}