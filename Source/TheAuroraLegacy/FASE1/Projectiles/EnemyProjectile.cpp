#include "EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../../TheAuroraLegacyPawn.h"
#include "TimerManager.h"

AEnemyProjectile::AEnemyProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    // Configuración de colisión optimizada
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
    CollisionSphere->InitSphereRadius(12.f);
    CollisionSphere->SetCollisionProfileName(TEXT("Projectile")); // Usa el canal de proyectiles de UE
    RootComponent = CollisionSphere;

    // Visual del proyectil
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // El mesh no necesita colisión, ya la tiene la esfera

    // Binding del evento Hit
    CollisionSphere->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);

    // Agregar mesh visible al proyectil
    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> SphereMesh(
            TEXT("/Engine/BasicShapes/Sphere.Sphere"));

    if (SphereMesh.Succeeded())
    {
        ProjectileMesh->SetStaticMesh(
            SphereMesh.Object);
        ProjectileMesh->SetWorldScale3D(
            FVector(0.1f, 0.1f, 0.1f));
    }
    // Tiempo de vida automático para evitar fugas de memoria si no golpea nada
    //InitialLifeSpan = 2.5f;
}

void AEnemyProjectile::BeginPlay()
{
    Super::BeginPlay();

}

void AEnemyProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsHidden()) return; // No moverse si está desactivado

    FVector NextLocation = GetActorLocation() +
        GetActorForwardVector() * Speed * DeltaTime;
    SetActorLocation(NextLocation);
}

void AEnemyProjectile::DeactivateSelf()
{
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetActorLocation(FVector::ZeroVector);
}

void AEnemyProjectile::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    // Detectar al jugador y hacerle daño
    ATheAuroraLegacyPawn* Player =
        Cast<ATheAuroraLegacyPawn>(OtherActor);

    if (Player)
    {
        // Hacer daño al jugador
        Player->TakeDamage_Ship(Damage);
        UE_LOG(LogTemp, Warning,
            TEXT("Proyectil impacto al jugador"));
    }

    // Desactivar en lugar de destruir
    // para que el Pool lo reutilice
    /*/
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetActorLocation(FVector::ZeroVector);
    /*/
}