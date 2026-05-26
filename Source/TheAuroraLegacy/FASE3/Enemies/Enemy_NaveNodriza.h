// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Enemies/EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "../StateMachine/Phase3State.h"
#include "Enemy_NaveNodriza.generated.h"

UCLASS()
class THEAURORALEGACY_API AEnemy_NaveNodriza : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemy_NaveNodriza();
    virtual void Tick(float DeltaTime) override;

    // Componente visual
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // Clase del proyectil del boss
    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<AActor> ProjectileClass;

    // Funciones del State Machine
    void SwitchToProtected();
    void SwitchToVulnerable();

    // Funciones del Decorator
    void ActivateShield();
    void DeactivateShield();

    // Disparar en 8 direcciones
    void FireInAllDirections();

    // Si tiene escudo no recibe daño
    bool bShieldActive = false;

    virtual void TakeDamageEnemy(int32 DamageAmount);

protected:
    virtual void BeginPlay() override;
    virtual void MoveEnemy(float DeltaTime) override;
    virtual void OnDeath() override;

private:
    // Estado actual
    UPROPERTY()
    UPhase3State* CurrentState;

    // Estados disponibles
    UPROPERTY()
    UPhase3State* ProtectedStateObj;

    UPROPERTY()
    UPhase3State* VulnerableStateObj;

    // Movimiento de oscilacion
    float OscillationSpeed = 200.f;
    float OscillationRange = 800.f;
    float OscillationTimer = 0.f;
    bool bGoingRight = true;
};