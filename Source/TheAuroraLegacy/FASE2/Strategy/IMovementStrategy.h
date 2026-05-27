#pragma once
#include "CoreMinimal.h"

class IMovementStrategy
{
public:
    virtual ~IMovementStrategy() {}

    // Cada estrategia implementa su propio movimiento
    virtual FVector CalculateMovement(
        FVector CurrentLocation,
        FVector PlayerLocation,
        float DeltaTime,
        float MoveSpeed) = 0;
}; 
