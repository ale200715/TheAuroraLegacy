#include "EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../TheAuroraLegacyPawn.h"

AEnemyProjectile::AEnemyProjectile()
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
        this, &AEnemyProjectile::OnHit);

    InitialLifeSpan = 3.f;
    Direction = FVector::ForwardVector;
}

void AEnemyProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation() +
        (Direction * ProjectileSpeed * DeltaTime);
    SetActorLocation(NewLocation, true);
}

void AEnemyProjectile::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    // Verificar si golpeo al jugador
    ATheAuroraLegacyPawn* Player = Cast<ATheAuroraLegacyPawn>(OtherActor);
    if (Player)
    {
        Player->TakeDamage_Ship(Damage);
        UE_LOG(LogTemp, Warning,
            TEXT("Proyectil enemigo golpeo al jugador! Daño: %d"), Damage);
    }

    Destroy();
}
