#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Ignorar colisi�n con el mundo para que no se atasque
    SetActorEnableCollision(true);
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    FVector EnemyLocation = GetActorLocation();
    FVector PlayerLocation = PlayerPawn->GetActorLocation();

    // Ignorar diferencia de altura
    PlayerLocation.Z = EnemyLocation.Z;

    FVector Direction = PlayerLocation - EnemyLocation;
    Direction.Normalize();

    // Rotar hacia el jugador
    FRotator NewRotation = Direction.ToOrientationRotator();
    SetActorRotation(NewRotation);

    // Mover hacia el jugador en plano horizontal
    FVector NewLocation = EnemyLocation + (Direction * MoveSpeed * DeltaTime);
    SetActorLocation(NewLocation, false);
}

void AEnemyBase::TakeDamageEnemy(int32 DamageAmount)
{
    Health -= DamageAmount;

    UE_LOG(LogTemp, Warning, TEXT("Enemigo recibio da�o. Vida restante: %d"), Health);

    if (Health <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemigo destruido!"));
        Destroy();
    }
}
