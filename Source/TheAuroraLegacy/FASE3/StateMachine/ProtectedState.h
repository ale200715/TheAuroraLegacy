// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StateMachine/Phase3State.h"
#include "ProtectedState.generated.h"

UCLASS()
class THEAURORALEGACY_API UProtectedState : public UPhase3State
{
    GENERATED_BODY()

public:
    virtual void EnterState(AActor* Owner) override;
    virtual void UpdateState(AActor* Owner,
        float DeltaTime) override;
    virtual void ExitState(AActor* Owner) override;

private:
    // Temporizador del estado protegido
    float StateTimer = 0.f;
    // Duracion del estado protegido: 15 segundos
    float StateDuration = 15.f;
    // Temporizador entre disparos
    float FireTimer = 0.f;
    // Intervalo entre disparos en 8 direcciones
    float FireInterval = 1.f;
};
