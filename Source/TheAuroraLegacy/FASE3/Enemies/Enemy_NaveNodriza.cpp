// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_NaveNodriza.h"
#include "Kismet/GameplayStatics.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "../StateMachine/ProtectedState.h"
#include "../StateMachine/VulnerableState.h"
#include "../../Core/GameFacade.h"

AEnemy_NaveNodriza::AEnemy_NaveNodriza()
{
    PrimaryActorTick.bCanEverTick = true;

    // Stats segun el documento
    Health = 20;
    ContactDamage = 3;
    ScoreValue = 1000;
    MoveSpeed = 200.f; // Muy lenta

    // Componente visual
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Empieza con escudo activo
    bShieldActive = true;
}

void AEnemy_NaveNodriza::BeginPlay()
{
    Super::BeginPlay();

    // Crear los estados
    ProtectedStateObj = NewObject<UProtectedState>(this);
    VulnerableStateObj = NewObject<UVulnerableState>(this);

    // Empezar en estado protegido
    CurrentState = ProtectedStateObj;
    CurrentState->EnterState(this);
}

void AEnemy_NaveNodriza::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Actualizar estado actual
    if (CurrentState)
    {
        CurrentState->UpdateState(this, DeltaTime);
    }
}

void AEnemy_NaveNodriza::MoveEnemy(float DeltaTime)
{
    // Oscilar izquierda y derecha
    FVector MyLocation = GetActorLocation();

    if (bGoingRight)
    {
        MyLocation.X += MoveSpeed * DeltaTime;
        if (MyLocation.X > OscillationRange)
            bGoingRight = false;
    }
    else
    {
        MyLocation.X -= MoveSpeed * DeltaTime;
        if (MyLocation.X < -OscillationRange)
            bGoingRight = true;
    }

    SetActorLocation(MyLocation);
}

void AEnemy_NaveNodriza::SwitchToProtected()
{
    if (CurrentState)
        CurrentState->ExitState(this);

    CurrentState = ProtectedStateObj;
    CurrentState->EnterState(this);
}

void AEnemy_NaveNodriza::SwitchToVulnerable()
{
    if (CurrentState)
        CurrentState->ExitState(this);

    CurrentState = VulnerableStateObj;
    CurrentState->EnterState(this);
}

void AEnemy_NaveNodriza::ActivateShield()
{
    bShieldActive = true;
    UE_LOG(LogTemp, Warning,
        TEXT("Boss: Escudo [ACTIVADO]"));
}

void AEnemy_NaveNodriza::DeactivateShield()
{
    bShieldActive = false;
    UE_LOG(LogTemp, Warning,
        TEXT("Boss: Escudo [DESACTIVADO]"));
}

void AEnemy_NaveNodriza::FireInAllDirections()
{
    if (!ProjectileClass) return;

    FVector Origin = GetActorLocation();

    // Obtener direccion hacia el jugador
    APawn* Player = UGameplayStatics::GetPlayerPawn(
        GetWorld(), 0);
    if (!Player) return;

    FVector ToPlayer = (Player->GetActorLocation()
        - Origin).GetSafeNormal();

    // 8 direcciones basadas en donde esta el jugador
    TArray<FVector> Directions = {
        ToPlayer,
        FVector(ToPlayer.X + 1.f, ToPlayer.Y, 0.f)
            .GetSafeNormal(),
        FVector(ToPlayer.X - 1.f, ToPlayer.Y, 0.f)
            .GetSafeNormal(),
        FVector(ToPlayer.X, ToPlayer.Y,
            ToPlayer.Z + 0.5f).GetSafeNormal(),
        FVector(ToPlayer.X, ToPlayer.Y,
            ToPlayer.Z - 0.5f).GetSafeNormal(),
        FVector(ToPlayer.X + 0.7f, ToPlayer.Y,
            ToPlayer.Z + 0.5f).GetSafeNormal(),
        FVector(ToPlayer.X - 0.7f, ToPlayer.Y,
            ToPlayer.Z + 0.5f).GetSafeNormal(),
        FVector(ToPlayer.X, ToPlayer.Y + 1.f, 0.f)
            .GetSafeNormal()
    };

    for (FVector Dir : Directions)
    {
        FVector SpawnLocation = Origin + Dir * 150.f;
        FRotator SpawnRotation = Dir.Rotation();

        GetWorld()->SpawnActor<AActor>(
            ProjectileClass,
            SpawnLocation,
            SpawnRotation);
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Boss: Disparo en 8 direcciones"));
}

void AEnemy_NaveNodriza::OnDeath()
{
    // Notificar al Facade
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

    UE_LOG(LogTemp, Warning,
        TEXT("Boss: Nave Nodriza destruida!"));

    Destroy();
}

void AEnemy_NaveNodriza::TakeDamageEnemy(
    int32 DamageAmount)
{
    if (bShieldActive)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Boss: Escudo bloqueo el danio"));
        return;
    }

    // Sin escudo si recibe danio
    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning,
        TEXT("Boss: Vida restante: %d"), Health);

    if (Health <= 0)
    {
        OnDeath();
    }
}
