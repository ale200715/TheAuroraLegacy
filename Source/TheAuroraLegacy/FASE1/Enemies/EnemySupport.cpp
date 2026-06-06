// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySupport.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Projectiles/EnemyProjectile.h"
#include "../TheAuroraLegacyPawn.h"
#include "../Core/GameFacade.h"
#include "../TheAuroraLegacyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemySupport::AEnemySupport()
{
    PrimaryActorTick.bCanEverTick = true;

    EnemyMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("SupportMesh"));
    RootComponent = EnemyMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SupportMeshAsset( TEXT("/Engine/BasicShapes/Sphere.Sphere"));

    if (SupportMeshAsset.Succeeded())
    {
        EnemyMesh->SetStaticMesh(SupportMeshAsset.Object);
        EnemyMesh->SetWorldScale3D( FVector(0.6f, 0.6f, 0.6f));
    }

    EnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EnemyMesh->SetCollisionProfileName(TEXT("BlockAll"));

    Health = 2;
    MoveSpeed = 250.f;
    ContactDamage = 1;
    ScoreValue = 250;
}

void AEnemySupport::BeginPlay()
{
    Super::BeginPlay();

    CachePlayer();
    FindPool();

    GetWorldTimerManager().SetTimer(FireTimerHandle,this, &AEnemySupport::FireProjectile,FireRate, true);
}

void AEnemySupport::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckSlowRadius();
}

void AEnemySupport::MoveEnemy(float DeltaTime)
{
    if (bIsSlowingPlayer && CachedPlayer.IsValid())
    {
        ATheAuroraLegacyPawn* PlayerPawn =Cast<ATheAuroraLegacyPawn>(CachedPlayer.Get());
        if (PlayerPawn && OriginalPlayerSpeed > 0.f) { 
            PlayerPawn->SetMaxSpeed(OriginalPlayerSpeed); 
        }
        bIsSlowingPlayer = false;
    }

    FVector NewLocation = GetActorLocation();
    NewLocation.Y += MoveDirection *MoveSpeed * DeltaTime;

    if (NewLocation.Y > HorizontalLimit)
        MoveDirection = -1.f;
    else if (NewLocation.Y < -HorizontalLimit)
        MoveDirection = 1.f;

    SetActorLocation(NewLocation);
}

void AEnemySupport::CheckSlowRadius()
{
    if (!CachedPlayer.IsValid())
    {
        CachePlayer();
        return;
    }

    float Distance = FVector::Dist( GetActorLocation(), CachedPlayer->GetActorLocation());

    ATheAuroraLegacyPawn* PlayerPawn =
        Cast<ATheAuroraLegacyPawn>(
            CachedPlayer.Get());

    if (!PlayerPawn) return;

    if (Distance <= SlowRadius && !bIsSlowingPlayer)
    {
        OriginalPlayerSpeed = PlayerPawn->GetMaxSpeed();
        bIsSlowingPlayer = true;
        PlayerPawn->SetMaxSpeed( OriginalPlayerSpeed * 0.5f);

        UE_LOG(LogTemp, Warning,TEXT("Support: Jugador ralentizado"));
    }
    else if (Distance > SlowRadius && bIsSlowingPlayer)
    {
        bIsSlowingPlayer = false;
        PlayerPawn->SetMaxSpeed(OriginalPlayerSpeed);

        UE_LOG(LogTemp, Warning, TEXT("Support: Velocidad restaurada"));
    }
}

void AEnemySupport::FireProjectile()
{
    if (!LevelPool)
    {
        FindPool();
        return;
    }

    AEnemyProjectile* Bullet = LevelPool->GetProjectileFromPool();

    if (!Bullet) return;

    FRotator ShootRotation = GetActorRotation();
    if (CachedPlayer.IsValid())
    {
        FVector Direction =CachedPlayer->GetActorLocation() - GetActorLocation();
        Direction.Normalize();
        ShootRotation = Direction.Rotation();
    }

    FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, -50.f);

    Bullet->SetActorLocation(SpawnLocation);
    Bullet->SetActorRotation(ShootRotation);
    Bullet->Speed = 600.f;
    Bullet->SetActorHiddenInGame(false);
    Bullet->SetActorTickEnabled(true);
    Bullet->SetActorEnableCollision(true);

    FTimerHandle DeactivateTimer;
    FTimerDelegate DeactivateDelegate;
    DeactivateDelegate.BindUObject( Bullet, &AEnemyProjectile::DeactivateSelf);

    GetWorldTimerManager().SetTimer( DeactivateTimer,DeactivateDelegate, 5.f,false);
}

void AEnemySupport::OnDeath()
{
    UE_LOG(LogTemp, Warning,TEXT("Support muerto - regresando al pool"));

    if (bIsSlowingPlayer && CachedPlayer.IsValid())
    {
        ATheAuroraLegacyPawn* PlayerPawn =Cast<ATheAuroraLegacyPawn>(CachedPlayer.Get());
        if (PlayerPawn) {
            PlayerPawn->SetMaxSpeed(
                OriginalPlayerSpeed);
        }

    }

    bIsSlowingPlayer = false;

    OriginalPlayerSpeed = 0.f;

    Super::OnDeath();

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetActorLocation(FVector::ZeroVector);
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

    Health = 2;
}

void AEnemySupport::RestartFireTimer()
{
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

    GetWorldTimerManager().SetTimer( FireTimerHandle, this, &AEnemySupport::FireProjectile, FireRate, true);
}

void AEnemySupport::CachePlayer()
{
    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Player) {
        CachedPlayer = Player;
    }
}

void AEnemySupport::FindPool()
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(),APhase1EnemyPool::StaticClass());

    LevelPool = Cast<APhase1EnemyPool>(FoundActor);
}
