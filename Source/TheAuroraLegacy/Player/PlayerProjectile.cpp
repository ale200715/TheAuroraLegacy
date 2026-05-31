
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

<<<<<<< HEAD
=======
    // LineTrace para detectar colisiones
>>>>>>> origin/fase2-camila
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

<<<<<<< HEAD
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, NextLocation,  ECollisionChannel::ECC_Visibility, QueryParams);

    DrawDebugLine( GetWorld(), StartLocation, NextLocation, FColor::Red, false, 0.1f);
=======
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        NextLocation,
        ECollisionChannel::ECC_Visibility,
        QueryParams);

    // Línea de debug visible en pantalla
    // para verificar que el raycast funciona
    DrawDebugLine(
        GetWorld(),
        StartLocation,
        NextLocation,
        FColor::Red,
        false,
        0.1f);
>>>>>>> origin/fase2-camila

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

<<<<<<< HEAD
=======
    // Mover el proyectil
>>>>>>> origin/fase2-camila
    SetActorLocation(NextLocation);
}