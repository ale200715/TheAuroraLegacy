// Fill out your copyright notice in the Description page of Project Settings.


#include "Interceptor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

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

    // Habilitar colision para recibir disparos
    MeshComponent->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionResponseToChannel(
        ECC_Visibility, ECR_Block);
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
        0.f).GetSafeNormal();

    FVector NewLocation = MyLocation +
        DiagonalDir * MoveSpeed * DeltaTime;
    SetActorLocation(NewLocation);
}