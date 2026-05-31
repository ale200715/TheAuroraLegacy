#include "ZigZagStrategy.h"
#include "Math/UnrealMathUtility.h"

FVector ZigZagStrategy::CalculateMovement(
    FVector CurrentLocation,
    FVector PlayerLocation,
    float DeltaTime,
    float MoveSpeed)
{
    ZigZagTimer += DeltaTime;

    // Dirección hacia el jugador
    FVector Direction = PlayerLocation - CurrentLocation;
    Direction.Normalize();

    // Movimiento perpendicular para el ZigZag
    FVector Perpendicular = FVector(-Direction.Y, Direction.X, 0.f);
    float ZigZag = FMath::Sin(ZigZagTimer * ZigZagFrequency) * ZigZagAmplitude;

    FVector NewLocation = CurrentLocation +
        (Direction * MoveSpeed * DeltaTime) +
        (Perpendicular * ZigZag * DeltaTime);

    return NewLocation;
}