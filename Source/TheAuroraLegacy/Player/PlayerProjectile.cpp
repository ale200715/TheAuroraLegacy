#include "PlayerProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBase.h"

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
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Proyectil impacto con: %s"),
            *OtherActor->GetName());

        // Verificar si golpeó un enemigo
        AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
        if (Enemy)
        {
            Enemy->TakeDamageEnemy(Damage);
            UE_LOG(LogTemp, Warning,
                TEXT("Enemigo recibio %d de daño"), Damage);
        }

        Destroy();
    }
}