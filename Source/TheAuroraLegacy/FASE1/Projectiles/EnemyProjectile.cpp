
#include "EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../../TheAuroraLegacyPawn.h"
#include "TimerManager.h"

AEnemyProjectile::AEnemyProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
    CollisionSphere->InitSphereRadius(12.f);

    // WorldDynamic para no colisionar con el suelo (WorldStatic)
    CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    // Solo detectar al Pawn
    CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionSphere->SetGenerateOverlapEvents(true);

    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnOverlapBegin);

    RootComponent = CollisionSphere;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Game/SpaceStation/Models/Shapecraft/Weapons/SM_Missile_1.SM_Missile_1'"));
    if (SphereMesh.Succeeded())
    {
        ProjectileMesh->SetStaticMesh(SphereMesh.Object);
        ProjectileMesh->SetWorldScale3D(FVector(0.1f));
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

    FVector Next = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
    // sweep=false — el overlap lo maneja el SphereComponent
    SetActorLocation(Next, false);
}

void AEnemyProjectile::OnOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;
    if (IsHidden()) return; // ya fue desactivado este frame

    if (ATheAuroraLegacyPawn* Player = Cast<ATheAuroraLegacyPawn>(OtherActor))
    {
        Player->TakeDamage_Ship(Damage);
        UE_LOG(LogTemp, Warning,TEXT("Proyectil impacto jugador: %d danio"), Damage);
        DeactivateSelf();
    }
}

void AEnemyProjectile::ScheduleDeactivation(float Delay)
{
    GetWorldTimerManager().ClearTimer(DeactivateTimerHandle);
    GetWorldTimerManager().SetTimer( DeactivateTimerHandle, this, &AEnemyProjectile::DeactivateSelf, Delay,false);
}

void AEnemyProjectile::DeactivateSelf()
{
    GetWorldTimerManager().ClearTimer(DeactivateTimerHandle);
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    // NO mover a ZeroVector — ese era el bug del piso
}