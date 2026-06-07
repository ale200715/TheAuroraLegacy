// Fill out your copyright notice in the Description page of Project Settings.


#include "TorretaProjectile.h"
#include "../../TheAuroraLegacyPawn.h"

ATorretaProjectile::ATorretaProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject
        <USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(15.f);
    CollisionSphere->SetCollisionObjectType(
        ECC_WorldDynamic);
    CollisionSphere->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);
    CollisionSphere->SetCollisionResponseToAllChannels(
        ECR_Ignore);
    CollisionSphere->SetCollisionResponseToChannel(
        ECC_Pawn, ECR_Overlap);
    CollisionSphere->SetGenerateOverlapEvents(true);
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(
        this, &ATorretaProjectile::OnOverlapBegin);
    RootComponent = CollisionSphere;

    MeshComponent = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);

    // Se destruye solo despues de 3 segundos
    InitialLifeSpan = 6.f;
}

void ATorretaProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ATorretaProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Mover hacia adelante
    FVector Next = GetActorLocation() +
        GetActorForwardVector() *
        ProjectileSpeed * DeltaTime;

    SetActorLocation(Next, false);
}

void ATorretaProjectile::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ATheAuroraLegacyPawn* Player =
        Cast<ATheAuroraLegacyPawn>(OtherActor);
    if (Player)
    {
        Player->TakeDamage_Ship(Damage);
        UE_LOG(LogTemp, Warning,
            TEXT("Torreta golpeo al jugador"));
        Destroy();
    }
}
