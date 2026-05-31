// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "../../Enemies/EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy_Torreta.generated.h"

UCLASS()
class THEAURORALEGACY_API AEnemy_Torreta : public AEnemyBase
{
    GENERATED_BODY()

public:
    AEnemy_Torreta();
    virtual void Tick(float DeltaTime) override;

    void ActivateTurret();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components")
    UStaticMeshComponent* MeshComponent;

protected:
    virtual void BeginPlay() override;
    virtual void MoveEnemy(float DeltaTime) override;
    virtual void OnDeath() override;

private:
    // Referencia al jugador
    AActor* PlayerActor;

    // Timer para disparar
    FTimerHandle FireTimerHandle;

    // Clase del proyectil que dispara
    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<AActor> ProjectileClass;

    // Funcion que dispara
    void FireAtPlayer();

    // Rotar para apuntar al jugador
    void RotateToPlayer(float DeltaTime);
};
