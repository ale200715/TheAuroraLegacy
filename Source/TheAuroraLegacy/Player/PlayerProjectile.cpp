
#include "PlayerProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBase.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

APlayerProjectile::APlayerProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject
        <USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(15.f);
    CollisionSphere->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);
    RootComponent = CollisionSphere;

    ProjectileMesh = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);

    InitialLifeSpan = 3.f;
}

void APlayerProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector StartLocation = GetActorLocation();
    FVector NextLocation = StartLocation +
        GetActorForwardVector() *
        ProjectileSpeed * DeltaTime;

    // LineTrace para detectar colisiones
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        NextLocation,
        ECollisionChannel::ECC_Visibility,
        QueryParams);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();

        UE_LOG(LogTemp, Warning,
            TEXT("Proyectil toco: %s"),
            *HitActor->GetName());

        AEnemyBase* Enemy =
            Cast<AEnemyBase>(HitActor);

        if (Enemy)
        {
            Enemy->TakeDamageEnemy(Damage);
            UE_LOG(LogTemp, Warning,
                TEXT("Enemigo recibio daño"));
        }

        Destroy();
        return;
    }

    // MOVER el proyectil hacia adelante
    SetActorLocation(NextLocation);
}