// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StateMachine/Phase3State.h"
#include "VulnerableState.generated.h"

UCLASS()
class THEAURORALEGACY_API UVulnerableState : public UPhase3State
{
    GENERATED_BODY()

public:
    virtual void EnterState(AActor* Owner) override;
    virtual void UpdateState(AActor* Owner,
        float DeltaTime) override;
    virtual void ExitState(AActor* Owner) override;

private:
    // Temporizador del estado vulnerable
    float StateTimer = 0.f;
    // Duracion del estado vulnerable: 4 segundos
    float StateDuration = 4.f;
};