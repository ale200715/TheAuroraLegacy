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

    EnemyMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("HunterMesh"));
    RootComponent = EnemyMesh;

    static ConstructorHelpers::FObjectFinder <UStaticMesh> HunterMeshAsset( TEXT("StaticMesh'/Game/SpaceStation/Models/Shapecraft/C8X/SM_C8X.SM_C8X'"));

    if (HunterMeshAsset.Succeeded())
    {
        EnemyMesh->SetStaticMesh( HunterMeshAsset.Object);
        EnemyMesh->SetWorldScale3D( FVector(0.05f, 0.05f, 0.05f));
    }

    EnemyMesh->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics);
    EnemyMesh->SetCollisionProfileName(TEXT("BlockAll"));

    Health = 1;
    MoveSpeed = 150.f;
    ContactDamage = 1;
    ScoreValue = 200;
}

void AEnemyHunter::BeginPlay()
{
    Super::BeginPlay();

    CachePlayer();
    FindPool();

    GetWorldTimerManager().SetTimer(  FireTimerHandle, this, &AEnemyHunter::StartBurst,  FireRate, true);
}

void AEnemyHunter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyHunter::MoveEnemy(float DeltaTime)
{
    if (!CachedPlayer.IsValid())
    {
        CachePlayer();
        return;
    }

    float Distance = FVector::Dist( GetActorLocation(), CachedPlayer->GetActorLocation());

    if (Distance <= 400.f) {
        return;
    }

    FVector Direction = CachedPlayer->GetActorLocation() - GetActorLocation(); Direction.Normalize();

    FVector NewLocation = GetActorLocation() + Direction * MoveSpeed * DeltaTime;

    SetActorLocation(NewLocation);
    SetActorRotation(Direction.Rotation());

}


void AEnemyHunter::StartBurst()
{
    BurstCount = 0;

    GetWorldTimerManager().SetTimer( BurstTimerHandle, this, &AEnemyHunter::FireBurst, 0.2f,true);
}

void AEnemyHunter::FireBurst()
{
    if (BurstCount >= BurstMax)
    {
        GetWorldTimerManager().ClearTimer( BurstTimerHandle);
        return;
    }

    BurstCount++;

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

    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f;

    Bullet->SetActorLocation(SpawnLocation);
    Bullet->SetActorRotation(ShootRotation);
    Bullet->SetActorHiddenInGame(false);
    Bullet->SetActorTickEnabled(true);
    Bullet->SetActorEnableCollision(true);

    FTimerHandle DeactivateTimer;
    FTimerDelegate DeactivateDelegate;
    DeactivateDelegate.BindUObject( Bullet, &AEnemyProjectile::DeactivateSelf);

    GetWorldTimerManager().SetTimer(DeactivateTimer,DeactivateDelegate, 3.f,false);

}

void AEnemyHunter::OnDeath()
{
    UE_LOG(LogTemp, Warning,TEXT("Hunter muerto - regresando al pool"));

    Super::OnDeath();

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetActorLocation(FVector::ZeroVector);
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
    GetWorldTimerManager().ClearTimer( BurstTimerHandle);

    Health = 2;
}

void AEnemyHunter::CachePlayer()
{
    APawn* Player =UGameplayStatics::GetPlayerPawn( GetWorld(), 0);
    if (Player) {
        CachedPlayer = Player;
    }
}

void AEnemyHunter::FindPool()
{
    AActor* FoundActor =UGameplayStatics::GetActorOfClass( GetWorld(),APhase1EnemyPool::StaticClass());

    LevelPool = Cast<APhase1EnemyPool>(FoundActor);
}

void AEnemyHunter::RestartFireTimer()
{
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
    GetWorldTimerManager().ClearTimer(BurstTimerHandle);

    BurstCount = 0;

    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemyHunter::StartBurst,FireRate, true);

    UE_LOG(LogTemp, Warning,TEXT("Hunter: Timer de disparo reiniciado"));
}
