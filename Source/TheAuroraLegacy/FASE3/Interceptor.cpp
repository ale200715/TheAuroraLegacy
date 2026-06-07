// Fill out your copyright notice in the Description page of Project Settings.


#include "Interceptor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "../TheAuroraLegacyPawn.h"
#include "../Core/GameFacade.h"

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

        // No dar score por contacto
        if (OwnerPool)
        {
            Health = 3;
            OwnerPool->ReturnToPool(this);
        }
        else
        {
            Destroy();
        }
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
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
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

            // No llamar OnDeath() — el contacto no da score
            // Solo devolver al pool sin notificar al Facade
            if (OwnerPool)
            {
                Health = 3;
                OwnerPool->ReturnToPool(this);
            }
            else
            {
                Destroy();
            }
        }
    }
}

void AInterceptor::OnDeath()
{
    // Notificar al Facade para score
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

    // Devolver al pool en lugar de destruirse
    if (OwnerPool)
    {
        // Resetear vida
        Health = 3;
        OwnerPool->ReturnToPool(this);
    }
    else
    {
        Destroy();
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Interceptor devuelto al pool"));
}