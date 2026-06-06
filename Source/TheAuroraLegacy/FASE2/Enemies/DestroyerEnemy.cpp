#include "DestroyerEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "../Projectiles/Phase2EnemyProjectile.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "../AuroraGameInstance.h"

ADestroyerEnemy::ADestroyerEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    Health = 6;
    MoveSpeed = 200.f;
}

void ADestroyerEnemy::BeginPlay()
{
    Super::BeginPlay();

    OnDestroyed.AddDynamic(this, &ADestroyerEnemy::OnDestroyerDestroyed);

    // Activar escudo cada 8 segundos
    GetWorldTimerManager().SetTimer(
        ShieldTimerHandle,
        this,
        &ADestroyerEnemy::ActivateShield,
        ShieldInterval,
        true
    );

    // Atacar cada 3 segundos
    GetWorldTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &ADestroyerEnemy::TripleAttack,
        AttackInterval,
        true
    );
}

void ADestroyerEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADestroyerEnemy::ChangeState(EDestroyerState NewState)
{
    CurrentState = NewState;

    switch (NewState)
    {
    case EDestroyerState::Normal:
        UE_LOG(LogTemp, Warning, TEXT("Destroyer: Estado NORMAL"));
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White,
                TEXT("Destroyer: Escudo desactivado"));
        break;

    case EDestroyerState::Shield:
        UE_LOG(LogTemp, Warning, TEXT("Destroyer: Estado ESCUDO"));
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue,
                TEXT("Destroyer: Escudo activado!"));
        break;

    case EDestroyerState::Attack:
        UE_LOG(LogTemp, Warning, TEXT("Destroyer: Estado ATAQUE"));
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
                TEXT("Destroyer: Disparo triple!"));
        break;

    case EDestroyerState::Dead:
        UE_LOG(LogTemp, Warning, TEXT("Destroyer: Estado MUERTO"));
        break;
    }
}

void ADestroyerEnemy::ActivateShield()
{
    ChangeState(EDestroyerState::Shield);

    // Desactivar colision - proyectiles lo atraviesan
    SetActorEnableCollision(false);

    GetWorldTimerManager().SetTimer(
        ShieldEndTimerHandle,
        this,
        &ADestroyerEnemy::DeactivateShield,
        ShieldDuration,
        false
    );
}

void ADestroyerEnemy::DeactivateShield()
{
    ChangeState(EDestroyerState::Normal);

    // Reactivar colision
    SetActorEnableCollision(true);
}

void ADestroyerEnemy::TripleAttack()
{
    if (CurrentState == EDestroyerState::Dead) return;
    if (!ProjectileClass) return;

    ChangeState(EDestroyerState::Attack);

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    // Direccion principal hacia el jugador
    FVector MainDirection = Player->GetActorLocation() - GetActorLocation();
    MainDirection.Normalize();

    // 3 direcciones: centro, izquierda, derecha
    TArray<FVector> Directions;
    Directions.Add(MainDirection);
    Directions.Add(MainDirection.RotateAngleAxis(30.f, FVector::UpVector));
    Directions.Add(MainDirection.RotateAngleAxis(-30.f, FVector::UpVector));

    for (FVector Dir : Directions)
    {
        FVector SpawnLocation = GetActorLocation() + Dir * 100.f;
        FRotator SpawnRotation = Dir.ToOrientationRotator();

        APhase2EnemyProjectile* Projectile = GetWorld()->SpawnActor
            <APhase2EnemyProjectile>(ProjectileClass,
                SpawnLocation, SpawnRotation);

        if (Projectile)
        {
            Projectile->Direction = Dir;
            UE_LOG(LogTemp, Warning,
                TEXT("Destroyer disparo triple!"));
        }
    }

    // Volver a Normal
    FTimerHandle ReturnTimer;
    GetWorldTimerManager().SetTimer(
        ReturnTimer,
        [this]() {
            if (IsValid(this) && CurrentState != EDestroyerState::Dead)
                ChangeState(EDestroyerState::Normal);
        },
        0.5f,
        false
    );
}

void ADestroyerEnemy::TakeDamageDestroyer(int32 DamageAmount)
{
    // Si tiene escudo no recibe danio
    if (CurrentState == EDestroyerState::Shield)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue,
                TEXT("Escudo bloqueo el danio!"));
        UE_LOG(LogTemp, Warning, TEXT("Escudo bloqueo el danio!"));
        return;
    }

    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("Destroyer recibio danio. Vida: %d"), Health);

    if (Health <= 0)
    {
        ChangeState(EDestroyerState::Dead);
        NotifyDeath();
        Destroy();
    }
}

void ADestroyerEnemy::NotifyDeath()
{
    UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(
        GetGameInstance());
    if (GI)
    {
        GI->AddScore(200);
        UE_LOG(LogTemp, Warning, TEXT("Destroyer destruido! +200 puntos"));
    }
    ATheAuroraLegacyGameMode* GM = Cast<ATheAuroraLegacyGameMode>(
        GetWorld()->GetAuthGameMode());
    if (GM)
    {
        GM->OnEnemyDefeated(200);
    }
}

void ADestroyerEnemy::OnDestroyerDestroyed(AActor* DestroyedActor)
{
    NotifyDeath();
}
