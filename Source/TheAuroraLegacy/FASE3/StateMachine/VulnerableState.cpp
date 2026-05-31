// Fill out your copyright notice in the Description page of Project Settings.


#include "VulnerableState.h"
#include "../Enemies/Enemy_NaveNodriza.h"

void UVulnerableState::EnterState(AActor* Owner)
{
    StateTimer = 0.f;

    // Desactivar escudo al entrar vulnerable
    AEnemy_NaveNodriza* Boss =
        Cast<AEnemy_NaveNodriza>(Owner);
    if (Boss)
    {
        Boss->DeactivateShield();
    }
}

void UVulnerableState::UpdateState(AActor* Owner,
    float DeltaTime)
{
    StateTimer += DeltaTime;

    // Cambiar a protegido despues de 3 segundos
    if (StateTimer >= StateDuration)
    {
        AEnemy_NaveNodriza* Boss =
            Cast<AEnemy_NaveNodriza>(Owner);
        if (Boss)
        {
            Boss->SwitchToProtected();
        }
    }
}

void UVulnerableState::ExitState(AActor* Owner)
{
    // Reactivar escudo al salir de vulnerable
    AEnemy_NaveNodriza* Boss =
        Cast<AEnemy_NaveNodriza>(Owner);
    if (Boss)
    {
        Boss->ActivateShield();
    }

}

