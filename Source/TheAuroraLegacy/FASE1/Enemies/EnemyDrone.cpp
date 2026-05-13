// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDrone.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

AEnemyDrone::AEnemyDrone()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> DroneMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    
    if (DroneMeshAsset.Succeeded())
    {
        EnemyMesh->SetStaticMesh(DroneMeshAsset.Object);
    }
  
    if (DroneMeshAsset.Succeeded() && EnemyMesh)
    {
        EnemyMesh->SetStaticMesh(DroneMeshAsset.Object);
        // Le damos un color o escala para diferenciarlo de la esfera base
        EnemyMesh->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));
    }

    // Stats según la tabla de tu fase
    Health = 1;
    MoveSpeed = 350.f;
    ContactDamage = 1;
    ScoreValue = 100;
}

void AEnemyDrone::BeginPlay()
{
    Super::BeginPlay();

    // Cachear jugador una sola vez
    CachePlayer();

    // Fijar dirección de movimiento al inicio
    // Línea recta hacia adelante
    MoveDirection = GetActorForwardVector() * -1.f;

    // Iniciar disparo periódico
    GetWorldTimerManager().SetTimer( FireTimerHandle, this, &AEnemyDrone::FireProjectile, FireRate, true); // true = repetir

}
void AEnemyDrone::Tick(float DeltaTime)
{
    // Solo llamamos al padre. El padre llamará a MoveEnemy por nosotros.
    Super::Tick(DeltaTime);
}


void AEnemyDrone::MoveEnemy(float DeltaTime)
{
    float DistanciaVaiven = 600.f;
    float VelocidadVaiven = 2.5f;

    float Offset = FMath::Sin(GetWorld()->GetTimeSeconds() * VelocidadVaiven) * DistanciaVaiven;


    FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * Offset * DeltaTime);

    SetActorLocation(NewLocation);
}
void AEnemyDrone::FireProjectile()
{
    if (!CachedPlayer.IsValid())
    {
        CachePlayer();
        return;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Drone: Disparando"));

   
}

void AEnemyDrone::CachePlayer()
{
    APawn* Player =
        UGameplayStatics::GetPlayerPawn(
            GetWorld(), 0);

    if (Player)
    {
        CachedPlayer = Player;
    }
}