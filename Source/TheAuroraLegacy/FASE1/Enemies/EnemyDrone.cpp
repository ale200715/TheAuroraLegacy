
#include "EnemyDrone.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Pool/Phase1EnemyPool.h"
#include "../Projectiles/EnemyProjectile.h"
#include "../Core/GameFacade.h"
#include "../TheAuroraLegacyGameMode.h"

AEnemyDrone::AEnemyDrone()
{
    PrimaryActorTick.bCanEverTick = true;

    EnemyMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("DroneMesh"));
    RootComponent = EnemyMesh;

    static ConstructorHelpers::FObjectFinder <UStaticMesh> DroneMeshAsset(TEXT("StaticMesh'/Game/SpaceStation/Models/Shapecraft/PRXProbe/SM_Probe1.SM_Probe1'"));

    if (DroneMeshAsset.Succeeded())
    {
        EnemyMesh->SetStaticMesh(DroneMeshAsset.Object);
        EnemyMesh->SetWorldScale3D( FVector(0.1f, 0.1f, 0.1f));
    }
   
    EnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EnemyMesh->SetCollisionProfileName(TEXT("BlockAll"));

    Health = 1;
    MoveSpeed = 350.f;
    ContactDamage = 1;
    ScoreValue = 100;

}

void AEnemyDrone::BeginPlay()
{
    Super::BeginPlay();
    CachePlayer();
    FindPool();

    MoveDirection = FVector(0.f, 1.f, 0.f);

    GetWorldTimerManager().SetTimer( FireTimerHandle,this,&AEnemyDrone::FireProjectile, FireRate,true);
}

void AEnemyDrone::CachePlayer()
{
    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Player) {
        CachedPlayer = Player;
    }
}


void AEnemyDrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyDrone::MoveEnemy(float DeltaTime)
{
    if (!CachedPlayer.IsValid())
    {
        CachePlayer();
        return;
    }

    FVector NewLocation = GetActorLocation() + MoveDirection * MoveSpeed * DeltaTime;

    if (NewLocation.Y > 700.f)
    {
        MoveDirection.Y = -1.f;
    }
    else if (NewLocation.Y < -700.f)
    {
        MoveDirection.Y = 1.f;
    }

    SetActorLocation(NewLocation);
}

void AEnemyDrone::FireProjectile()
{
    if (!LevelPool)
    {
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APhase1EnemyPool::StaticClass());
        LevelPool = Cast<APhase1EnemyPool>(FoundActor);
    }

    AEnemyProjectile* Bullet = LevelPool->GetProjectileFromPool();

    FVector SpawnLocation = GetActorLocation() + FVector(100.f, 0.f, 0.f);

    Bullet->SetActorLocation(SpawnLocation);
    Bullet->SetActorRotation(FRotator(0.f, 180.f, 0.f));

    Bullet->SetActorHiddenInGame(false);
    Bullet->SetActorTickEnabled(true);
    Bullet->SetActorEnableCollision(true);

    Bullet->ScheduleDeactivation(3.f);

}

void AEnemyDrone::RestartFireTimer()
{
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

    MoveDirection = FVector(0.f, 1.f, 0.f);

    GetWorldTimerManager().SetTimer(FireTimerHandle,this, &AEnemyDrone::FireProjectile,FireRate, true);
}


void AEnemyDrone::OnDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("Drone muerto - regresando al pool"));
   
    Super::OnDeath();

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetActorLocation(FVector::ZeroVector);
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

    Health = 1;
}

void AEnemyDrone::FindPool()
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APhase1EnemyPool::StaticClass());

    LevelPool = Cast<APhase1EnemyPool>(FoundActor);
}
