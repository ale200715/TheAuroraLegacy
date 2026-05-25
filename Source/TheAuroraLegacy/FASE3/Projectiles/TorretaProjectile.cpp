// Fill out your copyright notice in the Description page of Project Settings.


#include "TorretaProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../../TheAuroraLegacyPawn.h"

ATorretaProjectile::ATorretaProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject
        <USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(15.f);
    CollisionSphere->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);
    RootComponent = CollisionSphere;

    MeshComponent = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);

    // Se destruye solo despues de 3 segundos
    InitialLifeSpan = 3.f;
}

void ATorretaProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ATorretaProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Mover hacia adelante
    FVector StartLocation = GetActorLocation();
    FVector NextLocation = StartLocation +
        GetActorForwardVector() *
        ProjectileSpeed * DeltaTime;

    // LineTrace para detectar al jugador
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        NextLocation,
        ECC_Visibility,
        QueryParams);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();

        // Verificar si golpeo al jugador
        ATheAuroraLegacyPawn* Player =
            Cast<ATheAuroraLegacyPawn>(HitActor);
        if (Player)
        {
            Player->TakeDamage_Ship(Damage);
            UE_LOG(LogTemp, Warning,
                TEXT("Nave recibio danio"));
            Destroy();
            return;
        }
        Destroy();
        return;
    }

    SetActorLocation(NextLocation);
}

