// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Torreta.h"
#include "Kismet/GameplayStatics.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "../../Core/GameFacade.h"

AEnemy_Torreta::AEnemy_Torreta()
{
    PrimaryActorTick.bCanEverTick = true;

    // Stats segun el documento
    Health = 6;
    ContactDamage = 1;
    ScoreValue = 500;
    MoveSpeed = 0.f; // Fija, no se mueve

    // Componente visual
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void AEnemy_Torreta::BeginPlay()
{
    Super::BeginPlay();

    // Buscar al jugador
    PlayerActor = UGameplayStatics::GetPlayerPawn(
        GetWorld(), 0);

    // No iniciar disparo si esta oculta en el pool
    if (!IsHidden())
    {
        GetWorldTimerManager().SetTimer(
            FireTimerHandle,
            this,
            &AEnemy_Torreta::FireAtPlayer,
            1.f,
            true
        );
    }
}

void AEnemy_Torreta::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RotateToPlayer(DeltaTime);
}

void AEnemy_Torreta::MoveEnemy(float DeltaTime)
{
    // La torreta no se mueve, solo rota
    // La rotacion se maneja en Tick
}

void AEnemy_Torreta::RotateToPlayer(float DeltaTime)
{
    if (!PlayerActor) return;

    FVector Direction = PlayerActor->GetActorLocation()
        - GetActorLocation();
    FRotator TargetRotation = Direction.Rotation();

    FRotator NewRotation = FMath::RInterpTo(
        GetActorRotation(),
        TargetRotation,
        DeltaTime,
        5.f // velocidad de rotacion
    );

    SetActorRotation(NewRotation);
}

void AEnemy_Torreta::FireAtPlayer()
{
    if (!PlayerActor) return;
    if (!ProjectileClass) return;

    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();

    GetWorld()->SpawnActor<AActor>(
        ProjectileClass,
        SpawnLocation,
        SpawnRotation);

    UE_LOG(LogTemp, Warning,
        TEXT("Torreta disparo!"));
}

void AEnemy_Torreta::OnDeath()
{
    // Detener disparo
    GetWorldTimerManager().ClearTimer(FireTimerHandle);

    // Notificar al Facade para que sume el score
    TArray<AActor*> FoundFacades;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AGameFacade::StaticClass(),
        FoundFacades);

    if (FoundFacades.Num() > 0)
    {
        AGameFacade* Facade =
            Cast<AGameFacade>(FoundFacades[0]);
        if (Facade)
        {
            Facade->NotifyEnemyDefeated(this);
        }
    }

    // Devolver al pool
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
    Health = 6;

    UE_LOG(LogTemp, Warning,
        TEXT("Torreta devuelta al pool"));
}

void AEnemy_Torreta::ActivateTurret()
{
    PlayerActor = UGameplayStatics::GetPlayerPawn(
        GetWorld(), 0);

    GetWorldTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AEnemy_Torreta::FireAtPlayer,
        1.f,
        true
    );

    UE_LOG(LogTemp, Warning,
        TEXT("Torreta activada"));
}