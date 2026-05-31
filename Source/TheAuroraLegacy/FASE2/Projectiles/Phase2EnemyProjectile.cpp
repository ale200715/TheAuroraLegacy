#include "Phase2EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../TheAuroraLegacyPawn.h"

APhase2EnemyProjectile::APhase2EnemyProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject
        <USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(10.f);
    CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = CollisionSphere;

    ProjectileMesh = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);

    CollisionSphere->OnComponentHit.AddDynamic(
        this, &APhase2EnemyProjectile::OnHit);

    InitialLifeSpan = 3.f;
    Direction = FVector::ForwardVector;
}

void APhase2EnemyProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void APhase2EnemyProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation() +
        (Direction * ProjectileSpeed * DeltaTime);
    SetActorLocation(NewLocation, true);
}

void APhase2EnemyProjectile::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    ATheAuroraLegacyPawn* Player = Cast<ATheAuroraLegacyPawn>(OtherActor);
    if (Player)
    {
        Player->TakeDamage_Ship(Damage);
        UE_LOG(LogTemp, Warning,
            TEXT("Proyectil fase2 golpeo al jugador! Danio: %d"), Damage);
    }

    Destroy();
} 
