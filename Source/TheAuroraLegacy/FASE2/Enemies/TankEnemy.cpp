#include "TankEnemy.h"
#include "../Strategy/ZigZagStrategy.h"
#include "../GameModes/GameMode_Level5.h"
#include "Kismet/GameplayStatics.h"
#include "../Projectiles/EnemyProjectile.h"
#include "../AuroraGameInstance.h"

ATankEnemy::ATankEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    Health = 3;
    MoveSpeed = 450.f;

    // Inicializar Strategy
    ZigZag = new ZigZagStrategy();
    MovementStrategy = ZigZag;
}

void ATankEnemy::BeginPlay()
{
    Super::BeginPlay();

    OnDestroyed.AddDynamic(this, &ATankEnemy::OnTankDestroyed);

    GetWorldTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &ATankEnemy::FireAtPlayer,
        FireRate,
        true
    );
}

void ATankEnemy::Tick(float DeltaTime)
{
    // Usar Strategy para el movimiento ZigZag
    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player || !MovementStrategy) return;

    FVector NewLocation = MovementStrategy->CalculateMovement(
        GetActorLocation(),
        Player->GetActorLocation(),
        DeltaTime,
        MoveSpeed
    );

    SetActorLocation(NewLocation, false);

    // Rotar hacia el jugador
    FVector Direction = Player->GetActorLocation() - GetActorLocation();
    Direction.Normalize();
    SetActorRotation(Direction.ToOrientationRotator());
}

void ATankEnemy::FireAtPlayer()
{
    if (!IsValid(this)) return;
    if (!ProjectileClass) return;

    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

    FVector Direction = Player->GetActorLocation() - GetActorLocation();
    Direction.Normalize();

    FVector SpawnLocation = GetActorLocation() + Direction * 100.f;
    FRotator SpawnRotation = Direction.ToOrientationRotator();

    AEnemyProjectile* Projectile = GetWorld()->SpawnActor
        <AEnemyProjectile>(ProjectileClass,
            SpawnLocation, SpawnRotation);

    if (Projectile)
    {
        Projectile->Direction = Direction;
        UE_LOG(LogTemp, Warning, TEXT("Tank disparo proyectil!"));
    }
}

void ATankEnemy::NotifyDeath()
{
    UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(
        GetGameInstance());
    if (GI)
    {
        GI->AddScore(150);
        UE_LOG(LogTemp, Warning, TEXT("Tank destruido! +150 puntos"));
    }

    AGameMode_Level5* GM = Cast<AGameMode_Level5>(
        GetWorld()->GetAuthGameMode());
    if (GM)
    {
        GM->OnEnemyDefeated();
    }
}

void ATankEnemy::OnTankDestroyed(AActor* DestroyedActor)
{
    if (ZigZag)
    {
        delete ZigZag;
        ZigZag = nullptr;
    }
    NotifyDeath();
}

