// Fill out your copyright notice in the Description page of Project Settings.

#include "NaveNodrizaProjectile.h"
#include "../../TheAuroraLegacyPawn.h"
#include "Kismet/GameplayStatics.h"

ANaveNodrizaProjectile::ANaveNodrizaProjectile()
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
        this, &ANaveNodrizaProjectile::OnOverlapBegin);
    RootComponent = CollisionSphere;

    MeshComponent = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);

    InitialLifeSpan = 15.f;
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

    FVector Next = GetActorLocation() +
        GetActorForwardVector() *
        ProjectileSpeed * DeltaTime;

    SetActorLocation(Next, false);
}

void ANaveNodrizaProjectile::OnOverlapBegin(
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
            TEXT("Boss golpeo al jugador"));
        Destroy();
    }
}
