
#include "EnemyDrone.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Projectiles/EnemyProjectile.h"

AEnemyDrone::AEnemyDrone()
{
    PrimaryActorTick.bCanEverTick = true;

    EnemyMesh = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("DroneMesh"));
    RootComponent = EnemyMesh;

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> DroneMeshAsset(
            TEXT("/Engine/BasicShapes/Cube.Cube"));

    if (DroneMeshAsset.Succeeded())
    {
        EnemyMesh->SetStaticMesh(
            DroneMeshAsset.Object);
        EnemyMesh->SetWorldScale3D(
            FVector(0.5f, 0.5f, 0.5f));
    }

    Health = 1;
    MoveSpeed = 350.f;
    ContactDamage = 1;
    ScoreValue = 100;
}

void AEnemyDrone::BeginPlay()
{
    Super::BeginPlay();

    CachePlayer();

    if (CachedPlayer.IsValid())
    {
        FVector Direction =
            CachedPlayer->GetActorLocation() -
            GetActorLocation();
        Direction.Normalize();
        MoveDirection = Direction;
    }
    else
    {
        MoveDirection =
            GetActorForwardVector() * -1.f;
    }

    // NO buscar el pool aquí todavía
    // Se busca justo antes de disparar

    GetWorldTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AEnemyDrone::FireProjectile,
        FireRate,
        true);
}

void AEnemyDrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyDrone::MoveEnemy(float DeltaTime)
{
    FVector NewLocation = GetActorLocation() +
        MoveDirection * MoveSpeed * DeltaTime;

    SetActorLocation(NewLocation);

    // Si se aleja más de 3000 unidades 
    // del jugador desactivarse
    if (CachedPlayer.IsValid())
    {
        float Distance = FVector::Dist(
            GetActorLocation(),
            CachedPlayer->GetActorLocation());

        if (Distance > 3000.f)
        {
            // Desactivar en lugar de destruir
            // para que el pool lo reutilice
            SetActorHiddenInGame(true);
            SetActorTickEnabled(false);
            SetActorEnableCollision(false);
            GetWorldTimerManager().ClearTimer(
                FireTimerHandle);

            UE_LOG(LogTemp, Warning,
                TEXT("Drone: Salio del rango "
                    "y se desactivo"));
        }
    }
}

void AEnemyDrone::FireProjectile()
{
    if (!LevelPool)
    {
        AActor* FoundActor =
            UGameplayStatics::GetActorOfClass(
                GetWorld(),
                APhase1EnemyPool::StaticClass());
        LevelPool = Cast<APhase1EnemyPool>(FoundActor);
    }

    if (!LevelPool) return;

    AEnemyProjectile* Bullet =
        LevelPool->GetProjectileFromPool();

    if (!Bullet) return;

    FVector SpawnLocation = GetActorLocation() +
        GetActorForwardVector() * 100.f;

    FRotator ShootRotation = GetActorRotation();
    if (CachedPlayer.IsValid())
    {
        FVector Direction =
            CachedPlayer->GetActorLocation() -
            GetActorLocation();
        Direction.Normalize();
        ShootRotation = Direction.Rotation();
    }

    Bullet->SetActorLocation(SpawnLocation);
    Bullet->SetActorRotation(GetActorRotation());
    Bullet->SetActorHiddenInGame(false);
    Bullet->SetActorTickEnabled(true);
    Bullet->SetActorEnableCollision(true);

    // Auto-desactivar después de 3 segundos
    // si no golpea nada
    FTimerHandle DeactivateTimer;
    FTimerDelegate DeactivateDelegate;
    DeactivateDelegate.BindUObject(
        Bullet,
        &AEnemyProjectile::DeactivateSelf);

    GetWorldTimerManager().SetTimer(
        DeactivateTimer,
        DeactivateDelegate,
        3.f,
        false);

    UE_LOG(LogTemp, Warning,
        TEXT("Drone: Proyectil disparado"));
}
void AEnemyDrone::CachePlayer()
{
    APawn* Player =
        UGameplayStatics::GetPlayerPawn(
            GetWorld(), 0);
    if (Player)
        CachedPlayer = Player;
}