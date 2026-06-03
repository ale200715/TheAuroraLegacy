// Fill out your copyright notice in the Description page of Project Settings.


#include "Interceptor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "../TheAuroraLegacyPawn.h"

void AInterceptor::NotifyActorBeginOverlap(
    AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    ATheAuroraLegacyPawn* Player =
        Cast<ATheAuroraLegacyPawn>(OtherActor);
    if (Player)
    {
        Player->TakeDamage_Ship(ContactDamage);
        UE_LOG(LogTemp, Warning,
            TEXT("Interceptor golpeo al jugador"));

        // Destruirse al impactar
        Destroy();
    }
}

AInterceptor::AInterceptor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Stats del Interceptor segun el documento
    Health = 3;
    ContactDamage = 2;
    ScoreValue = 300;
    MoveSpeed = 300.f; // Velocidad muy alta

    // Componente visual
    MeshComponent = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Colision
    SetActorEnableCollision(true);
    MeshComponent->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionObjectType(
        ECC_WorldDynamic);
    MeshComponent->SetCollisionResponseToAllChannels(
        ECR_Overlap);
    MeshComponent->SetCollisionResponseToChannel(
        ECC_Visibility, ECR_Block);
    MeshComponent->SetGenerateOverlapEvents(true);
}

void AInterceptor::BeginPlay()
{
    Super::BeginPlay();

    PlayerActor = UGameplayStatics::GetPlayerPawn(
        GetWorld(), 0);

    // Diagonal aleatoria
    bGoingRight = FMath::RandBool();

    MoveDirection = FVector(
        bGoingRight ? 1.f : -1.f,
        0.f,
        0.f
    ).GetSafeNormal();
}

void AInterceptor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AInterceptor::MoveEnemy(float DeltaTime)
{
    if (!PlayerActor) return;

    FVector PlayerLocation = PlayerActor->GetActorLocation();
    FVector MyLocation = GetActorLocation();

    // Direccion directa al jugador
    FVector ToPlayer = (PlayerLocation - MyLocation).GetSafeNormal();

    // Agregar componente diagonal fijo en X
    FVector DiagonalDir = FVector(
        ToPlayer.X + (bGoingRight ? 0.5f : -0.5f),
        ToPlayer.Y,
        ToPlayer.Z
       ).GetSafeNormal();

    FVector NewLocation = MyLocation +
        DiagonalDir * MoveSpeed * DeltaTime;
    SetActorLocation(NewLocation);

    // Detectar contacto por distancia
    float Distance = FVector::Dist(
        MyLocation, PlayerLocation);

    if (Distance < 150.f)
    {
        ATheAuroraLegacyPawn* Player =
            Cast<ATheAuroraLegacyPawn>(PlayerActor);
        if (Player)
        {
            Player->TakeDamage_Ship(ContactDamage);
            UE_LOG(LogTemp, Warning,
                TEXT("Interceptor contacto con jugador"));
            Destroy();
        }
    }
}