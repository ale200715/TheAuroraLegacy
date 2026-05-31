// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Phase3State.generated.h"

UCLASS(Abstract)
class THEAURORALEGACY_API UPhase3State : public UObject
{
    GENERATED_BODY()

public:
    // Llamado al entrar al estado
    virtual void EnterState(AActor* Owner) {}

    // Llamado cada frame
    virtual void UpdateState(AActor* Owner,
        float DeltaTime) {
    }

    // Llamado al salir del estado
    virtual void ExitState(AActor* Owner) {}
};
