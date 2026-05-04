#include "PlayerProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

APlayerProjectile::APlayerProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject
        <USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(15.f);
    RootComponent = CollisionSphere;

    ProjectileMesh = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);

    CollisionSphere->OnComponentHit.AddDynamic(
        this, &APlayerProjectile::OnHit);

    // La bala se destruye sola a los 3 segundos
    InitialLifeSpan = 3.f;
}

void APlayerProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Mover la bala hacia adelante
    FVector NewLocation = GetActorLocation() +
        GetActorForwardVector() *
        ProjectileSpeed * DeltaTime;

    SetActorLocation(NewLocation);
}

void APlayerProjectile::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    // Por ahora solo se destruye al impactar
    // Cuando crees EnemyBase agregaremos el daño aquí
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Proyectil impacto con: %s"),
            *OtherActor->GetName());

        Destroy();
    }
}