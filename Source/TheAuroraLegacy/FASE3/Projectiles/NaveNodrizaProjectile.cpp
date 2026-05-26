// Fill out your copyright notice in the Description page of Project Settings.

#include "NaveNodrizaProjectile.h"
#include "../../TheAuroraLegacyPawn.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

ANaveNodrizaProjectile::ANaveNodrizaProjectile()
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

    InitialLifeSpan = 8.f;
}

void ANaveNodrizaProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Ignorar colision con la nave nodriza
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AActor::StaticClass(),
        FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (Actor && Actor->GetClass()->GetName()
            .Contains(TEXT("NaveNodriza")))
        {
            CollisionSphere->IgnoreActorWhenMoving(
                Actor, true);
        }
    }
}

void ANaveNodrizaProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector StartLocation = GetActorLocation();
    FVector NextLocation = StartLocation +
        GetActorForwardVector() *
        ProjectileSpeed * DeltaTime;

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
        ATheAuroraLegacyPawn* Player =
            Cast<ATheAuroraLegacyPawn>(HitActor);
        if (Player)
        {
            Player->TakeDamage_Ship(Damage);
            UE_LOG(LogTemp, Warning,
                TEXT("Boss golpeo al jugador"));
        }
        Destroy();
        return;
    }

    SetActorLocation(NextLocation);
}

