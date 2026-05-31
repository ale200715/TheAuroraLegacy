#pragma once
#include "IMovementStrategy.h"

class ZigZagStrategy : public IMovementStrategy
{
public:
    virtual FVector CalculateMovement(
        FVector CurrentLocation,
        FVector PlayerLocation,
        float DeltaTime,
        float MoveSpeed) override;

private:
    float ZigZagTimer = 0.f;
    float ZigZagFrequency = 2.f;
    float ZigZagAmplitude = 300.f;
};
