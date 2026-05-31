// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtectedState.h"
#include "../Enemies/Enemy_NaveNodriza.h"

void UProtectedState::EnterState(AActor* Owner)
{
    StateTimer = 0.f;
    FireTimer = 0.f;
}

void UProtectedState::UpdateState(AActor* Owner,
    float DeltaTime)
{
    StateTimer += DeltaTime;
    FireTimer += DeltaTime;

    // Disparar en 8 direcciones cada FireInterval
    if (FireTimer >= FireInterval)
    {
        FireTimer = 0.f;
        AEnemy_NaveNodriza* Boss =
            Cast<AEnemy_NaveNodriza>(Owner);
        if (Boss)
        {
            Boss->FireInAllDirections();
        }
    }

    // Cambiar a vulnerable despues de 5 segundos
    if (StateTimer >= StateDuration)
    {
        AEnemy_NaveNodriza* Boss =
            Cast<AEnemy_NaveNodriza>(Owner);
        if (Boss)
        {
            Boss->SwitchToVulnerable();
        }
    }
}

void UProtectedState::ExitState(AActor* Owner)
{
    
}
