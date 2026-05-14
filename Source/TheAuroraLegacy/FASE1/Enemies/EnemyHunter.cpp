// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHunter.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Projectiles/EnemyProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Core/GameFacade.h"
#include "../TheAuroraLegacyGameMode.h"

AEnemyHunter::AEnemyHunter()
{
    PrimaryActorTick.bCanEverTick = true;

    EnemyMesh = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("HunterMesh"));
    RootComponent = EnemyMesh;

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> HunterMeshAsset(
            TEXT("/Engine/BasicShapes/Cone.Cone"));

    if (HunterMeshAsset.Succeeded())
    {
        EnemyMesh->SetStaticMesh(
            HunterMeshAsset.Object);
        EnemyMesh->SetWorldScale3D(
            FVector(0.5f, 0.5f, 0.5f));
    }

    // Configurar colisión
    EnemyMesh->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics);
    EnemyMesh->SetCollisionProfileName(
        TEXT("BlockAll"));

    // Stats según la tabla
    Health = 2;
    MoveSpeed = 450.f;
    ContactDamage = 1;
    ScoreValue = 200;
}

void AEnemyHunter::BeginPlay()
{
    Super::BeginPlay();

    CachePlayer();
    FindPool();

    // Iniciar ráfagas periódicas
    GetWorldTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AEnemyHunter::StartBurst,
        FireRate,
        true);
}

void AEnemyHunter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyHunter::MoveEnemy(float DeltaTime)
{
    // Perseguir al jugador activamente
    if (!CachedPlayer.IsValid())
    {
        CachePlayer();
        return;
    }

    FVector Direction =
        CachedPlayer->GetActorLocation() -
        GetActorLocation();
    Direction.Normalize();

    FVector NewLocation = GetActorLocation() +
        Direction * MoveSpeed * DeltaTime;

    SetActorLocation(NewLocation);
    SetActorRotation(Direction.Rotation());

    // Desactivar si se aleja demasiado
    float Distance = FVector::Dist(
        GetActorLocation(),
        CachedPlayer->GetActorLocation());

    if (Distance > 5000.f)
    {
        SetActorHiddenInGame(true);
        SetActorTickEnabled(false);
        SetActorEnableCollision(false);
        GetWorldTimerManager().ClearTimer(
            FireTimerHandle);
        GetWorldTimerManager().ClearTimer(
            BurstTimerHandle);
    }
}

void AEnemyHunter::StartBurst()
{
    BurstCount = 0;

    GetWorldTimerManager().SetTimer(
        BurstTimerHandle,
        this,
        &AEnemyHunter::FireBurst,
        0.2f,
        true);
}

void AEnemyHunter::FireBurst()
{
    if (BurstCount >= BurstMax)
    {
        GetWorldTimerManager().ClearTimer(
            BurstTimerHandle);
        return;
    }

    BurstCount++;

    if (!LevelPool)
    {
        FindPool();
        return;
    }

    AEnemyProjectile* Bullet =
        LevelPool->GetProjectileFromPool();

    if (!Bullet) return;

    // Disparar hacia el jugador
    FRotator ShootRotation = GetActorRotation();
    if (CachedPlayer.IsValid())
    {
        FVector Direction =
            CachedPlayer->GetActorLocation() -
            GetActorLocation();
        Direction.Normalize();
        ShootRotation = Direction.Rotation();
    }

    FVector SpawnLocation = GetActorLocation() +
        GetActorForwardVector() * 100.f;

    Bullet->SetActorLocation(SpawnLocation);
    Bullet->SetActorRotation(ShootRotation);
    Bullet->SetActorHiddenInGame(false);
    Bullet->SetActorTickEnabled(true);
    Bullet->SetActorEnableCollision(true);

    // Auto desactivar después de 3 segundos
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
        TEXT("Hunter: Disparo %d de %d"),
        BurstCount, BurstMax);
}

void AEnemyHunter::OnDeath()
{
    UE_LOG(LogTemp, Warning,
        TEXT("Hunter muerto - regresando al pool"));

    // Notificar al Facade
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
            Facade->NotifyEnemyDefeated(this);
    }

    // Notificar al GameMode
    ATheAuroraLegacyGameMode* GM =
        Cast<ATheAuroraLegacyGameMode>(
            GetWorld()->GetAuthGameMode());
    if (GM)
        GM->OnEnemyDefeated(ScoreValue);

    // Desactivar en lugar de destruir
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetActorLocation(FVector::ZeroVector);
    GetWorldTimerManager().ClearTimer(
        FireTimerHandle);
    GetWorldTimerManager().ClearTimer(
        BurstTimerHandle);

    // Restaurar vida para reutilización
    Health = 2;
}

void AEnemyHunter::CachePlayer()
{
    APawn* Player =
        UGameplayStatics::GetPlayerPawn(
            GetWorld(), 0);
    if (Player)
        CachedPlayer = Player;
}

void AEnemyHunter::FindPool()
{
    AActor* FoundActor =
        UGameplayStatics::GetActorOfClass(
            GetWorld(),
            APhase1EnemyPool::StaticClass());

    LevelPool = Cast<APhase1EnemyPool>(FoundActor);
}

