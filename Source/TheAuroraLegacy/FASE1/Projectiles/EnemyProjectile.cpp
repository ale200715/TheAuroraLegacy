
#include "EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../../TheAuroraLegacyPawn.h"
#include "../../Enemies/EnemyBase.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h" 

AEnemyProjectile::AEnemyProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
    CollisionSphere->InitSphereRadius(12.f);

    CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));
   
    RootComponent = CollisionSphere;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    CollisionSphere->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        ProjectileMesh->SetStaticMesh(SphereMesh.Object);
        ProjectileMesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
    }
}


void AEnemyProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsHidden()) return;

    FVector NextLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
    SetActorLocation(NextLocation, true);
}

void AEnemyProjectile::DeactivateSelf()
{
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetActorLocation(FVector::ZeroVector);
}

void AEnemyProjectile::ScheduleDeactivation(float Delay)
{
    GetWorldTimerManager().ClearTimer(DeactivateTimerHandle);
    GetWorldTimerManager().SetTimer(DeactivateTimerHandle, this, &AEnemyProjectile::DeactivateSelf, Delay, false);
}

void AEnemyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    ATheAuroraLegacyPawn* Player = Cast<ATheAuroraLegacyPawn>(OtherActor);

    if (Player)
    {
        Player->TakeDamage_Ship(Damage);

        UE_LOG(LogTemp, Warning, TEXT("¡El proyectil impactó al jugador y causó %f de daño!"), Damage);

        DeactivateSelf(); 
    }
    else
    {
        DeactivateSelf();
    }
}
