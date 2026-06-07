// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TorretaProjectile.generated.h"

UCLASS()
class THEAURORALEGACY_API ATorretaProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATorretaProjectile();
    virtual void Tick(float DeltaTime) override;

    // Velocidad del proyectil
    UPROPERTY(EditAnywhere, Category = "Combat")
    float ProjectileSpeed = 800.f;

    // Daño que hace al jugador
    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 Damage = 1;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionSphere;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};
